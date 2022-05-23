#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;
using namespace chrono;

struct UI_Zahtev
{
    int id_proces;
    int staza;
    condition_variable cv;
    bool obradjen;
    UI_Zahtev(int id, int staza) : id_proces(id), staza(staza), obradjen(false) {}
};

class OS
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    condition_variable cv_obrada;
    queue<UI_Zahtev *> zahtevi;
    bool gotovo;

public:
    OS(Dijagnostika &d) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        gotovo = false;
    }

    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se uputio zahtev ka U/I uređaju
    //
    // id_procesa - ID procesa koji upućuje zahtev
    // broj_staze - broj staze diska kojoj nit želi da pristupi
    //
    // Metoda treba da formira novi zahtev i uputi ga U/I uređaju na obradu. Proces će biti blokiran dok god se ovaj zahtev ne izvrši.
    // Pre nego što stupi u blokadu, potrebno je pozvati dijagnostika.proces_ceka.
    void uputi_UI_zahtev(int id_procesa, int broj_staze)
    {
        // Implementirati ...
        UI_Zahtev *novi = new UI_Zahtev(id_procesa, broj_staze);
        unique_lock<mutex> l(m);
        zahtevi.push(novi);
        cv_obrada.notify_one();
        while (!(novi->obradjen))
        {
            dijagnostika.proces_ceka(id_procesa, broj_staze);
            novi->cv.wait(l);
        }
        delete novi;
    }

    // Metoda koju poziva nit koja simulira deo operativnog sistema koji upravlja diskom, kako bi se obradio jedan pristigli zahtev
    //
    // Prema uslovima zadatka, metoda treba da obradi zahtev koji je najranije pristigao.
    //
    // Povratna vrednost metode treba da bude broj staze koji je obrađen.
    int obradi_zahtev()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (zahtevi.empty() && !gotovo)
        {
            cv_obrada.wait(l);
        }
        while(gotovo) this_thread::sleep_for(seconds(1));
        UI_Zahtev* obradjen = zahtevi.front();
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        zahtevi.pop();
        obradjen->obradjen = true;
        obradjen->cv.notify_one();
        return obradjen->staza;
    }

    // Metoda koja postavlja signal za kraj izvršavanja i pokreće obrađivač iz čekanja kako bi program mogao da se završi
    void zavrsi()
    {
        // Implementirati ...
        gotovo = true;
        cv_obrada.notify_one();
    }
};

#endif // OS_H_INCLUDED
