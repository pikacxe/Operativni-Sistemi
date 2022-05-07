#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>


using namespace std;

// Implementacija pokretanja tri niti koje treba da pretraže (svaka svoju trećinu) ulazni vektor brojeva i nađu element najbliži nuli
//
// ulaz - Ulazni vektor brojeva koji se pretražuje
//
// Povratna vrednost je nađeni element, najbliži nuli od svih elemenata koji su obuhvaćeni pretragom
//
double izracunaj(vector<double> ulaz)
{
    int n = 3;
    int br_el_nit = ulaz.size() / 3;
    thread niti[3];
    double rez[n];
    for (int i = 0; i < n; ++i)
    {
        niti[i] = thread(pretrazi, ulaz.begin() + i * br_el_nit, ulaz.begin() + (i + 1) * br_el_nit,ref(rez[i]));
    }
    for (int i = 0; i < n; ++i)
    {
        niti[i].join();
    }
    double best = rez[0];
    for(int i = 1;i<n;i++){
        if(abs(best) > abs(rez[i])){
            best = rez[i];
        }
    }
    return best;
    // Implementirati ...
}

#endif // NITI_H
