#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

#include "dijagnostika.h"
#include "proces.h"
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <queue>

using namespace std;

struct UI_Zahtev
{
    bool obradjen;
    int process_id;
    condition_variable cv;
    UI_Zahtev(int pid) : process_id(pid), obradjen(false) {}
};

class OS
{
private:
    Dijagnostika &dijagnostika;
    queue<UI_Zahtev *> zahtevi;
    mutex m;
    condition_variable cv_process;
    condition_variable cv_ui;
    int aktivni_process;
    int max_instrukcija;
    bool end;

public:
    // d                 - Referenca na objekat Dijagnostika, koji služi za ispis stanja simulacije
    // maks_uzastopnih_i - Maksimalan dozvoljen broj uzastopnih STANDARD instrukcija, nakon kojeg obavezno sledi preključivanje
    OS(Dijagnostika &d, int maks_uzastopnih_i) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        max_instrukcija = maks_uzastopnih_i;
        aktivni_process = -1;
        end = false;
    }

    // Pomoćna metoda koja vraća objekat klase Dijagnostika koji služi za ispis stanja simulacije
    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira proces kako bi se izvršile sve instrukcije procesa.
    //
    // p - Struktura koja sadrži opis procesa (ID procesa i instrukcije koje treba izvršiti)
    //
    // Ukoliko proces ne može da se izvrši (procesor je zauzet), proces mora da se uveže u red spremnih i treba pozvati metodu dijagnostika.proces_ceka.
    // Ukoliko dođe do toga da izvršenje procesa prestaje (dostignut je maksimalan dozvoljen broj uzastopnih STANDARD instrukcija ili su sve instrukcije procesa izvršene), potrebno je pozvati dijagnostika.proces_oslobadja_procesor.
    // Kada se izvrši STANDARD instrukcija nekog procesa, potrebno je pozvati dijagnostika.proces_izvrsio_standardnu.
    // Kada se izvršava UI instrukcija, potrebno je pozvati par metoda: dijagnostika.proces_ceka_na_UI kada se pokrene U/I operacija (nakon čega sledi čekanje na U/I podsistem) i dijagnostika.proces_zavrsio_UI kada se završi U/I operacija.
    void izvrsi_proces(Proces &p)
    {
        // Implementirati ...
        int br_i = 0;
        for (auto it = p.instrukcije.begin(); it != p.instrukcije.end(); ++it)
        {
            unique_lock<mutex> l(m);
            while (aktivni_process != -1 && aktivni_process != p.id)
            {
                dijagnostika.proces_ceka(p.id);
                cv_process.wait(l);
            }
            aktivni_process = p.id;
            if ((*it) == STANDARD)
            {
                l.unlock();
                this_thread::sleep_for(chrono::milliseconds(rand() % 41 + 10));
                l.lock();
                dijagnostika.proces_izvrsio_standardnu(p.id);
                br_i++;
                if (br_i == max_instrukcija)
                {
                    br_i = 0;
                    aktivni_process = -1;
                    dijagnostika.proces_oslobadja_procesor(p.id);
                    cv_process.notify_one();
                    l.unlock();
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            }
            else if ((*it) == UI)
            {
                UI_Zahtev *z = new UI_Zahtev(p.id);
                zahtevi.push(z);
                dijagnostika.proces_ceka_na_UI(p.id);
                cv_ui.notify_one();
                br_i = 0;
                aktivni_process = -1;
                dijagnostika.proces_oslobadja_procesor(p.id);
                cv_process.notify_one();
                while (z->obradjen == false)
                {
                    z->cv.wait(l);
                }
                dijagnostika.proces_zavrsio_UI(p.id);
                delete z;
            }
        }
        unique_lock<mutex> l(m);
        aktivni_process = -1;
        cv_process.notify_one();
    }

    // Metoda koju poziva nit koja simulira obrađivač U/I zahteva kako bi se obradili trenutno pohranjeni U/I zahtevi.
    //
    // Potrebno je pozvati dijagnostika.ui_ceka kada ne postoje trenutno pohranjeni U/I zahtevi i obrađivač čeka na prvi.
    // Potrebno je pozvati dijagnostika.ui_zapocinje kada obrađivač krene u obradu U/I zahteva. Kada obrađivač završi taj zahtev, potrebno je pozvati dijagnostika.ui_zavrsio.
    void obradi_zahteve()
    {
        while (true)
        {
            // Implementirati ...
            unique_lock<mutex> l(m);
            while (zahtevi.empty() && end)
            {
                dijagnostika.ui_ceka();
                cv_ui.wait(l);
            }
            UI_Zahtev *trenutni = zahtevi.front();
            zahtevi.pop();
            dijagnostika.ui_zapocinje(trenutni->process_id);
            l.unlock();
            this_thread::sleep_for(chrono::milliseconds(rand() % 201 + 100));
            l.lock();
            trenutni->obradjen = true;
            dijagnostika.ui_zavrsio(trenutni->process_id);
            trenutni->cv.notify_one();
        }
    }
    void zavrsi(){
        end = true;
        cv_ui.notify_one();
    }
};

#endif // OS_H_INCLUDED
