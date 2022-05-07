#ifndef NITI_H_INCLUDED
#define NITI_H_INCLUDED

#include <vector>
#include <thread>

#include "suma_vektora.h"

using namespace std;

// v - vektor čije elemente treba sumirati
// n - koliko niti treba pokrenuti
// povratna vrednost - suma svih elemenata vektora, izračunata pokretanjem n niti (svaka nit treba da obradi jedan deo elemenata)
double sumiraj(vector<double> v, int n) {
    // Implementirati ...
    double sume[n];
    double suma = 0;
    for (int i = 0; i < n; ++i){
        sume[i] = 0;
    }
    int br_el = (int)(v.size() / n);
    thread niti[n];
    int k;
    for(k = 0; k < n-1; ++k){
        niti[k] = thread(f,v.begin()+ br_el*k, v.begin() + br_el*(k+1), ref(sume[k]));
    }
    niti[k] = thread(f,v.begin()+ br_el*k, v.end(), ref(sume[k]));
    for(int i = 0; i < n; ++i){
        niti[i].join();
    }
    for(int i = 0; i < n; ++i){
        suma += sume[i];
    }
    return suma;
}

#endif // NITI_H_INCLUDED
