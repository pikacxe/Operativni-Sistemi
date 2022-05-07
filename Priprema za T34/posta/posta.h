#ifndef POSTA_H_INCLUDED
#define POSTA_H_INCLUDED

#include "klijent.h"
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;
using namespace chrono;

class Posta {
private:
    Klijent& klijent;
    mutex m;
    condition_variable cv;
    bool salter1, salter2;
    int svota1, svota2;

public:
    Posta(Klijent& kl) : klijent(kl) {
        // Prosiriti po potrebi ...
        salter1 = salter2 = false;
        svota1 = svota2 = 0;
    }

    // Metoda koju poziva nit koja simulira klijenta kada on uplacuje neki iznos.
    // Metoda je blokirajuca - ako je salter zauzet, metoda ce stajati blokirana dok se on ne oslobodi.
    //
    // rbr   - Redni broj klijenta
    // svota - Svota koju klijent zeli da uplati
    //
    // Potrebno je pozvati metodu klijent.ceka kada su salteri zauzeti i klijent mora da ceka.
    // Potrebno je pozvati metodu klijent.uplacuje kada klijent stupi na salter i vrsi svoje placanje.
    // Potrebno je pozvati metodu klijent.napusta kada klijent zavrsi placanje i napusta salter.
    void uplati(int rbr, int svota) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(salter1 == true && salter2 == true) {
            klijent.ceka(rbr, svota);
            cv.wait(l);
        }
        int rbr_saltera;
        if(salter1 == false) {
            salter1 = true;
            rbr_saltera = 1;
        }
        else {
            salter2 = true;
            rbr_saltera = 2;
        }
        klijent.uplacuje(rbr,rbr_saltera,svota);
        l.unlock();
        this_thread::sleep_for(seconds(svota));
        l.lock();
        if(rbr_saltera == 1) {
            salter1 = false;
            svota1 += svota;
            klijent.napusta(rbr,rbr_saltera,svota1);
        }
        else {
            salter2 = false;
            svota2 += svota;
            klijent.napusta(rbr,rbr_saltera,svota2);
        }
        cv.notify_one();

    }
};

#endif // POSTA_H_INCLUDED
