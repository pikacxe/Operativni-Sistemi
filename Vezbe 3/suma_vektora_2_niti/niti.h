#ifndef NITI_H_INCLUDED
#define NITI_H_INCLUDED

#include <vector>
#include <thread>

#include "suma_vektora.h"

using namespace std;

// v - vektor čije elemente treba sumirati
// povratna vrednost - suma svih elemenata vektora, izračunata pokretanjem 2 niti (svaka treba da obradi jednu polovinu elemenata)
double sumiraj(vector<double> v) {
    // Implementirati ...
    double suma1 = 0, suma2 = 0;
    thread t1(f,v.begin(), v.begin()+v.size()/2, ref(suma1)),
    t2(f,v.begin()+v.size()/2,v.end(),ref(suma2));
    t1.join();
    t2.join();
    return suma1 + suma2;
}

#endif // NITI_H_INCLUDED
