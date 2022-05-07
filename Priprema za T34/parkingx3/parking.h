#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include "automobil.h"
#include <mutex>
#include <condition_variable>

using namespace std;

class Parking {
    Automobil& automobil;
    mutex m;
    condition_variable cv[3];
    int br_ulaza;
    bool parking_slobodan;
public:
    Parking(Automobil& a) : automobil(a) {
        // Prosiriti po potrebi ...
        br_ulaza = 0;
        parking_slobodan = true;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada on pokusava da udje na parking.
    // Metoda je blokirajuca - ukoliko nema slobodnog mesta, izvrsenje je u blokadi dok se ono ne oslobodi.
    //
    // rbr  - Redni broj automobila
    // ulaz - Ulaz na koji automobil pokusava da udje
    //
    // Potrebno je pozvati metodu automobil.ceka kada automobil ne moze da pridje parkingu i mora da ceka.
    // Potrebno je pozvati metodu automobil.zauzima kada automobil zauzme parking mesto.
    void udji(int rbr, int ulaz) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (parking_slobodan == false || ulaz != br_ulaza) {
            automobil.ceka(rbr, ulaz);
            cv[ulaz].wait(l);
        }
        parking_slobodan = false;
        br_ulaza = (br_ulaza + 1) % 3; //ako je ulaz 3, ponovo postavlja na 0
        automobil.zauzima(rbr);
        
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada on napusta parking na koji je prethodno usao.
    //
    // rbr  - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada automobil napusti parking mesto.
    void izadji(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        parking_slobodan = true;
        automobil.napusta(rbr);
        cv[br_ulaza].notify_one();
    }
};

#endif // PARKING_H_INCLUDED
