#ifndef NITI_H
#define NITI_H

#include <thread>
#include <map>

using namespace std;

// Implementacija pokretanja dodatne niti koja treba da pronađe reč suprotnog značenja (koristeći funkciju "nadjiAntonim")
//
// recnik   - Rečnik koji sadrži parove reč - reč suprotnog značenja
// rec      - Reč za koju se traži reč suprotnog značenja
//
// Povratna vrednost treba da bude reč suprotnog značenja (ako je pronađena)
//
string izracunaj(map<string, string> recnik, string rec)
{
    string rez;
    thread nit(nadjiAntonim,recnik,rec,ref(rez));
    nit.join();
    return rez;
    // Implementirati ...
}


#endif // NITI_H
