#ifndef PRODAVNICA_H_INCLUDED
#define PRODAVNICA_H_INCLUDED

#include "povratna.h"
#include "kupac.h"
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <random>

using namespace std;

class Prodavnica {
private:
    Kupac& kupac;
    mutex m;
    condition_variable cv;
    int slobodne_kabine;
public:
    Prodavnica(Kupac& k, int n): kupac(k) {
        // Prosiriti po potrebi ...
        slobodne_kabine = n;
    }

    // Metoda koju poziva nit koja simulira kupca, kada kupac obavlja soping (procedura je opisana u zaglavlju main.cpp)
    // Metoda je blokirajuca - ako kupac mora da ceka da se oslobodi kabina za probu, izvrsenje ce blokirati dok se za to ne steknu uslovi.
    //
    // rbr - Redni broj kupca
    //
    // Potrebno je pozvati metodu kupac.ceka kada su kabine zauzete i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.proba_odecu kada kupac udje u kabinu da proba odecu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio probu odece.
    povratna_vrednost kupi(int rbr) {
        // Implementirati ...
        steady_clock::time_point pocetak = steady_clock::now();
        unique_lock<mutex> l(m);
        while (slobodne_kabine == 0) {
            kupac.ceka(rbr);
            cv.wait(l);
        }
        slobodne_kabine--;
        kupac.proba_odecu(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(1));
        l.lock();
        povratna_vrednost pv;
        pv.kupio = rand()%2;
        pv.cekao_na_kabinu = steady_clock::now() - pocetak;
        kupac.zavrsio(rbr, pv);
        slobodne_kabine++;
        cv.notify_one();
        return pv;
    }

};

#endif // PRODAVNICA_H_INCLUDED
