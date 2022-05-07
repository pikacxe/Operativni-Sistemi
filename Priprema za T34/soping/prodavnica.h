#ifndef PRODAVNICA_H_INCLUDED
#define PRODAVNICA_H_INCLUDED

#include "povratna.h"
#include "kupac.h"

using namespace std;

class Prodavnica {
private:
    Kupac& kupac;
public:
    Prodavnica(Kupac& k, int n): kupac(k) {
        // Prosiriti po potrebi ...
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
    }
};

#endif // PRODAVNICA_H_INCLUDED
