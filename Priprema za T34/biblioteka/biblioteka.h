#ifndef BIBLIOTEKA_H_INCLUDED
#define BIBLIOTEKA_H_INCLUDED

#include "primerak.h"
#include <mutex>
#include <condition_variable>

using namespace std;

class Biblioteka {
private:
    Primerak& primerak;
    mutex m;
    condition_variable cv;
    int br_primeraka;
public:
    Biblioteka(Primerak& pr, int br) : primerak(pr) {
        // Prosiriti po potrebi ...
        br_primeraka = br;
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da iznajmi knjigu.
    // Metoda je blokirajuca - ukoliko nema slobodnih primeraka knjige, stajace u stanju cekanja dok se neki ne oslobodi.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.ceka kada nema slobodnih primeraka knjige.
    // Potrebno je pozvati metodu primerak.iznajmljen kada ima slobodnih primeraka knjige.
    void iznajmi(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (br_primeraka == 0) {
            primerak.ceka(rbr);
            cv.wait(l);
        }
        br_primeraka--;
        primerak.iznajmljen(rbr);
        
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da vrati knjigu koju je prethodno iznajmio.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.vracen kada je primerak vracen u biblioteku.
    void vrati(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        br_primeraka++;
        primerak.vracen(rbr);
        cv.notify_one();
    }
};

#endif // BIBLIOTEKA_H_INCLUDED
