#ifndef NITI_H
#define NITI_H

#include <thread>
#include <list>

using namespace std;

// Implementacija obrtanja redosleda elemenata u listi pokretanjem jedne nove niti
//
// ulazna  - Ulazna lista, koja se obrađuje
//
// Rezultat je rezultujuća lista (popunjena elementima u obrnutom redosledu)
//
list<int> izracunaj(const list<int>& ulazna)
{
    // Implementirati ...
    list<int> pom;
    thread nit(obrniRedosled, ulazna, ref(pom));
    nit.join();
    return pom;
}

#endif // NITI_H
