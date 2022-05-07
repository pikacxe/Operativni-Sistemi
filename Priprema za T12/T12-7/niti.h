#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>

using namespace std;

// Implementacija operacije XOR nad ulaznim vektorima A i B izvršene uz pomoć dve niti (svaka nit obrađuje polovinu)
//
// a - Vektor A (16 binarnih cifara)
// b - Vektor B (16 binarnih cifara)
//
// Rezultat izvršavanja treba da bude vector<int> koji sadrži rezultat izvršenja XOR operacije nad ulaznim vektorima.
//
vector<int> izracunaj(vector<int>& a, vector<int>& b)
{
    // Implementirati ...
    int n = a.size()/2;
    thread nit1,nit2;
    vector<int> c(a.size());
    nit1 = thread(izvrsiXOR,a.begin(), a.begin()+n, b.begin(), c.begin());
    nit2 = thread(izvrsiXOR,a.begin()+n, a.end(), b.begin()+n, c.begin()+n);
    nit1.join();
    nit2.join();
    return c;

}

#endif // NITI_H
