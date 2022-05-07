#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include "automobil.h"
#include <mutex>
#include <condition_variable>


using namespace std;

class Parking {
private:
    Automobil& automobil;
    int slobodnih_mesta;
    mutex m;
    condition_variable cv;

public:
    Parking(Automobil& a, int kapacitet) : automobil(a) {
        // Proširiti po potrebi ...
        slobodnih_mesta = kapacitet;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokušao da se parkira.
    // Ako je parking zauzet, izvršenje ove metode će trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto ne može da se parkira.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.
    void udji(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(slobodnih_mesta == 0){
            automobil.ceka(rbr);
            cv.wait(l);
        }
        automobil.parkira(rbr);
        slobodnih_mesta--;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon što je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napušta parking mesto.
    void izadji(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        automobil.napusta(rbr);
        slobodnih_mesta++;
        cv.notify_all();
    }
};

#endif // PARKING_H_INCLUDED
