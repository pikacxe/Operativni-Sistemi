#ifndef RACUNAR_H_INCLUDED
#define RACUNAR_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>

using namespace std;

class Racunar
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    condition_variable cv_rasporedjivac, cv;
    bool gotovo;
    bool rasporedi;
    int aktivan;
    map<int, int> procesi;

public:
    Racunar(Dijagnostika &d) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        gotovo = false;
        rasporedi = false;
        aktivan = -1;
    }

    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // trajanje     - trajanje procesa (izraženo u stotinama milisekundi)
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što se proces izvršio, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi(int id_procesa, int trajanje)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        procesi[id_procesa] = trajanje;
        while (aktivan != -1 && aktivan != id_procesa)
        {
            dijagnostika.proces_ceka(id_procesa);
            cv.wait(l);
        }
        l.unlock();
        this_thread::sleep_for(chrono::milliseconds(trajanje * 100));
        l.lock();
        dijagnostika.proces_zavrsio(id_procesa);
        rasporedi = true;
        cv_rasporedjivac.notify_one();
    }

    // Metoda koju poziva nit koja simulira raspoređivač operativnog sistema, kako bi se izabrao sledeći proces za izvršenje.
    //
    // Ukoliko još uvek ne postoji potreba da se raspoređivač aktivira, potrebno je pozvati metodu dijagnostika.rasporedjivac_ceka a nakon toga nit treba da pređe u stanje čekanja.
    // Nakon što se raspoređivač aktivirao i izabrao sledeći proces, potrebno je pozvati dijagnostika.rasporedjivac_izvrsio.
    void rasporedjuj()
    {
        while (true)
        {
            // Implementirati ...
            unique_lock<mutex> l(m);
            while (!rasporedi && !gotovo)
            {
                dijagnostika.rasporedjivac_ceka();
                cv_rasporedjivac.wait(l);
            }
            if (gotovo) break;
            auto min = procesi.begin();
            for (auto it = procesi.begin(); it != procesi.end(); ++it)
            {
                if (it->second < min->second)
                {
                    min = it;
                }
            }
            aktivan = min->first;
            procesi.erase(min);
            dijagnostika.rasporedjivac_izvrsio(aktivan);
            rasporedi = false;
            cv.notify_one();
        }
    }

    // Metoda koja postavlja signal za kraj izvršavanja i pokreće raspoređivač iz čekanja kako bi program mogao da se završi
    void zavrsi()
    {
        // Implementirati ...
        gotovo = true;
        rasporedi = true;
        cv_rasporedjivac.notify_one();
    }
};

#endif // RACUNAR_H_INCLUDED
