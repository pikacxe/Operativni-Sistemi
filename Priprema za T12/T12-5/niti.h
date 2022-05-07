#ifndef NITI_H
#define NITI_H

#include <thread>
#include <map>

using namespace std;

// Implementacija pokretanja dodatne niti koja treba da napravi novi rečnik (koristeći funkciju "napraviRecnik")
//
// brojNiti - Broj niti koje je potrebno startovati
map<string, string> izracunaj(map<string, string> engSrp)
{
    int brojNiti = 5;
    map<string, string> srpEng;
    thread nit(napraviRecnik,ref(engSrp),ref(srpEng));
    nit.join();
    return srpEng;
    // Implementirati ...
}

#endif // NITI_H
