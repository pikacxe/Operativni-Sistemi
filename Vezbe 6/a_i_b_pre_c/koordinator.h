#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

using namespace std;

#include <mutex>
#include <condition_variable>

class Koordinator {
private:
    const int cekam_niti;     // Konstanta koja govori koliko niti se čeka na završetak pre nego što se aktivira poslednja.
    condition_variable cv;
    mutex m;
    int zavrsene;
public:
    Koordinator(int cn) : cekam_niti(cn) {
        // Proširiti po potrebi ...
        zavrsene = 0;
    }

    // Metoda koju na svom KRAJU zovu niti (a i b) koje ne čekaju (izvršavaju se odmah).
    void zavrsio() {
        // Implementirati ...
        zavrsene++;
        if(cekam_niti == zavrsene){
            cv.notify_one();
        }
    }

    // Metodu koju na svom POČETKU zovu niti (c) koje čekaju (ne izvršavaju se odmah).
    void cekam() {
        // Implementirati ...
        unique_lock<mutex> l(m);
        cv.wait(l);
    }
};
#endif // KOORDINATOR_H_INCLUDED
