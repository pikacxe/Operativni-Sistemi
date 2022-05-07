#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include "automobil.h"
#include <mutex>
#include <condition_variable>

using namespace std;

class Parking
{
private:
    Automobil &automobil;
    mutex m;
    enum Stanje
    {
        ZAUZETO,
        SLOBODNO
    };
    Stanje stanje;
    condition_variable cv;

public:
    Parking(Automobil &a) : automobil(a)
    {
        // Proširiti po potrebi ...
        stanje = SLOBODNO;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokušao da se parkira.
    // Ako je parking zauzet, izvršenje ove metode će trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto mora da čeka.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.
    void udji(int rbr)
    {
        unique_lock<mutex> l(m);
        while (stanje == ZAUZETO)
        {
            automobil.ceka(rbr);
            cv.wait(l);
            // stanje == SLOBODNO
        }
        automobil.parkira(rbr);
        stanje= ZAUZETO;

    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon što je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napušta parking mesto.
    void izadji(int rbr)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        stanje = SLOBODNO;
        automobil.napusta(rbr);
        cv.notify_one();
    }
};

#endif // PARKING_H_INCLUDED
