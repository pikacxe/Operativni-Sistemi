#ifndef TRZNI_CENTAR_H_INCLUDED
#define TRZNI_CENTAR_H_INCLUDED

#include "kupac.h"
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class Trzni_centar
{
private:
    Kupac &kupac;
    mutex m;
    condition_variable cv_kasa[2];
    bool slobodne_kase[2];
    int kasa[2];

public:
    Trzni_centar(Kupac &k) : kupac(k)
    {
        // Prosiriti po potrebi ...
        slobodne_kase[0] = 1;
        slobodne_kase[1] = 1;
        kasa[0] = 0;
        kasa[1] = 0;
    }

    // Metoda koju poziva nit koja simulira kupca kako bi se obavila kupovina.
    // Metoda je blokirajuca - ako su sve kase zauzete, onda se izvrsenje blokira dok se neka ne oslobodi.
    //
    // rbr           - Redni broj kupca
    // broj_artikala - Broj artikala koje kupac zeli da kupi
    //
    // Potrebno je pozvati metodu kupac.ceka kada je izabrana kasa zauzeta i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.kupuje kada se kasa oslobodi i kupac pocinje kupovinu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio kupovinu.
    int kupi(int rbr, int broj_artikala)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        int id_kase = (kasa[0] + !slobodne_kase[0]) > (kasa[1] + !slobodne_kase[1]) ? 1 : 0;
        while (slobodne_kase[id_kase] == false)
        {
            kasa[id_kase]++;
            kupac.ceka(rbr, id_kase);
            cv_kasa[id_kase].wait(l);
        }
        slobodne_kase[id_kase] = false;
        kupac.kupuje(rbr, id_kase, broj_artikala);
        l.unlock();
        this_thread::sleep_for(chrono::seconds(broj_artikala));
        l.lock();
        kupac.zavrsio(rbr);
        kasa[id_kase]--;
        slobodne_kase[id_kase] = true;
        cv_kasa[id_kase].notify_one();
        return id_kase;
    }
};

#endif // TRZNI_CENTAR_H_INCLUDED
