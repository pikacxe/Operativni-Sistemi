#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

using namespace std;

#include <mutex>
#include <condition_variable>

class Koordinator
{
private:
    condition_variable cv;
    mutex m;
    bool zavrsio = false;

public:
    // Funkcija se poziva nakon što je prva nit završila svoj posao, kako bi signalizirala pokretanje druge niti.
    void prvi_zavrsio()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        zavrsio = true;
        cv.notify_one();
    }

    // Blokirajuća funkcija koja se poziva na početku druge niti kako bi nit čekala dok se prva nit ne završi.
    void drugi_ceka()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (zavrsio == false)
        {
            cv.wait(l);
        }
    }
};

#endif // KOORDINATOR_H_INCLUDED
