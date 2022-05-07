#ifndef NITI_H_INCLUDED
#define NITI_H_INCLUDED

#include <vector>
#include <thread>

#include "suma_vektora.h"

using namespace std;

// v - vektor A čije elemente treba sumirati
// v - vektor B čije elemente treba sumirati
// povratna vrednost - suma korespondentnih elemenata vektora A i B, izračunata pozivanjem dve niti
vector<double> sumiraj(vector<double> a, vector<double> b) {
    // Implementirati ...
    vector<double> suma_a(a.size()/2);
    vector<double> suma_b(a.size()/2);

    thread t1(f,a.begin(),a.begin() + a.size()/2, b.begin(), suma_a.begin());
    thread t2(f,a.begin()+a.size()/2, a.end(), b.begin()+ a.size()/2, suma_b.begin());
    t1.join();
    t2.join();
    vector<double> suma(a.size());
    vector<double>::iterator it = suma.begin();
    vector<double>::iterator ita = suma_a.begin();
    vector<double>::iterator itb = suma_b.begin();
    for(it; it!= suma.end(); ++it){
        if(ita != suma_a.end()){
            *it = *ita++;
        }
        else{
            *it = *itb++;
        }
    }
    return suma;
}

#endif // NITI_H_INCLUDED
