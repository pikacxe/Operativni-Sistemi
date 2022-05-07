#ifndef OBRTANJE_H
#define OBRTANJE_H

#include <list>

using namespace std;

// Implementacija obrtanja redosleda elemenata u listi
// Ova funkcija treba da bude iskorišćena za funkciju niti koje ovaj program pokreće.
//
// ulazna  - Ulazna lista, koja se obrađuje
// izlazna - Rezultujuća lista (biće popunjena elementima u obrnutom redosledu)
//
void obrniRedosled(const list<int>& ulazna, list<int>& izlazna)
{
    izlazna = ulazna;
    izlazna.pop_back();
    izlazna.push_front(ulazna.back());
    // Implementirati ...
}

#endif // OBRTANJE_H
