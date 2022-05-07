#ifndef TRZNI_CENTAR_H_INCLUDED
#define TRZNI_CENTAR_H_INCLUDED

#include "kupac.h"

using namespace std;

class Trzni_centar {
private:
    Kupac& kupac;
public:
    Trzni_centar(Kupac& k) : kupac(k) {
        // Prosiriti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira kupca kako bi se obavila kupovina.
    // Metoda je blokirajuca - ako su sve kase zauzete, onda se izvrsenje blokira dok se neka ne oslobodi.
    //
    // rbr           - Redni broj kupca
    // broj_artikala - Broj artikala koje kupac zeli da kupi
    //
    // Potrebno je pozvati metodu kupac.ceka kada je izabrana kasa zauzeta i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.kupuje kada se kasa oslobodi i kupac pocinje kupovinu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio kupovinu.
    int kupi(int rbr, int broj_artikala) {
        // Implementirati ...
    }
};


#endif // TRZNI_CENTAR_H_INCLUDED
