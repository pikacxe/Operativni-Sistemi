#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>
#include <vector>

using namespace std;

class Memorija {
private:
    mutex m;
    vector<char> mem;
public:
    Memorija(int bajtova) {
        // Implementirati ...
        mem.resize(bajtova,'-');
    }

    char citaj(int adresa) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        return mem[adresa];
    }

    void pisi(int adresa, char vrednost) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        mem[adresa] = vrednost;
    }
};

#endif // MEMORIJA_H_INCLUDED
