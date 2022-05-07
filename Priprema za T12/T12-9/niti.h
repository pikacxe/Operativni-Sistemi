#ifndef NITI_H
#define NITI_H

#include <thread>

using namespace std;

double izracunaj(list<string> kupljeno, map<string, double> cenovnik)
{
    // Implementirati ...
    double rez = 0;
    thread nit(obracunajUkupno,kupljeno,cenovnik,ref(rez));
    nit.join();
    return rez;
}

#endif // NITI_H
