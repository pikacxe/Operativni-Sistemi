#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

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
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        return mem[adresa];
    }

    void pisi(int adresa, char vrednost) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        mem[adresa] = vrednost;
    }
};

#endif // MEMORIJA_H_INCLUDED
