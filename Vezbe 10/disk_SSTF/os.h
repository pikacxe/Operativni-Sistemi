#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <algorithm>

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

class OS {
private:
    Dijagnostika& dijagnostika;
    bool gotovo;
    condition_variable cv_obrada;
    vector<UI_Zahtev*> zahtevi;
    mutex m;
    int trenutna_staza;

public:
    OS(Dijagnostika& d, int t) : dijagnostika(d) {
        // Proširiti po potrebi ...
        gotovo = false;
        trenutna_staza = t;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se uputio zahtev ka U/I uređaju
    //
    // id_procesa - ID procesa koji upućuje zahtev
    // broj_staze - broj staze diska kojoj nit želi da pristupi
    //
    // Metoda treba da formira novi zahtev i uputi ga U/I uređaju na obradu. Proces će biti blokiran dok god se ovaj zahtev ne izvrši. Pre nego što stupi u blokadu, potrebno je pozvati dijagnostika.proces_ceka.
    void uputi_UI_zahtev(int id_procesa, int broj_staze) {
        // Implementirati ...
        UI_Zahtev* novi = new UI_Zahtev(id_procesa, broj_staze);
        unique_lock<mutex> l(m);
        zahtevi.push_back(novi);
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
    // Prema uslovima zadatka, metoda treba da obradi zahtev kojim se pristupa onoj stazi koja je najbliža stazi na kojoj je trenutno glava diska
    //
    // Povratna vrednost metode treba da bude broj staze koji je obrađen.
    int obradi_zahtev() {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(zahtevi.empty() && !gotovo){
            cv_obrada.wait(l);
        }
        if(gotovo) return -1;
        int min_razlika = abs(zahtevi[0]->staza - trenutna_staza);
        auto it_pronadjen = zahtevi.begin();
        for(auto it = zahtevi.begin(); it != zahtevi.end(); it++){
            if(abs((*it)->staza - trenutna_staza) < min_razlika){
                min_razlika = abs((*it)->staza - trenutna_staza);
                it_pronadjen = it;
            }
        }
        UI_Zahtev* temp = *it_pronadjen;
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        zahtevi.erase(it_pronadjen);
        trenutna_staza = temp->staza;
        temp->obradjen = true;
        temp->cv.notify_one();
        return temp->staza;
    }

    // Metoda koja postavlja signal za kraj izvršavanja i pokreće obrađivač iz čekanja kako bi program mogao da se završi
    void zavrsi() {
        // Implementirati ...
        gotovo = true;
        cv_obrada.notify_one();
    }
};

#endif // OS_H_INCLUDED
