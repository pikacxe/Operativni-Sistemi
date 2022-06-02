#ifndef PROCESOR_H_INCLUDED
#define PROCESOR_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

class Procesor
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    condition_variable cv_process;
    condition_variable cv_ui;
    bool slobodan_processor, prekid;

public:
    Procesor(Dijagnostika &d) : dijagnostika(d)
    {
        // Proširiti po potrebi...
        slobodan_processor = true;
        prekid = false;
    }

    // Pomoćna metoda koja vraća referencu na objekat Dijagnostika asociran sa ovim objektom
    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira proces, kako bi se izvršile sve naredbe procesa.
    //
    // id           - id procesa
    // broj_naredbi - ukupan broj naredbi koje ovaj proces izvršava
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Kada proces uspe da zauzme procesor, pre izvršenja naredbe treba da se pozove dijagnostika.proces_izvrsava.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi_proces(int id, int broj_naredbi)
    {
        // Implementirati ...
        for (int i = 0; i < broj_naredbi; i++)
        {
            unique_lock<mutex> l(m);
            while (!slobodan_processor)
            {
                dijagnostika.proces_ceka(id);
                cv_process.wait(l);
            }
            slobodan_processor = false;
            dijagnostika.proces_izvrsava(id, i + 1);
            l.unlock();
            this_thread::sleep_for(chrono::seconds(1));
            l.lock();
            slobodan_processor = true;
            dijagnostika.proces_zavrsio(id, i + 1);
            if (prekid)
            {
                cv_ui.notify_one();
            }
            else
            {
                cv_process.notify_one();
            }
            l.unlock();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    // Metoda koju poziva nit koja simulira obrađivač U/I prekida kako bi se inicirao i obradio prekid.
    //
    // Ukoliko je procesor već zauzet i ne može se obaviti obrađivanje prekida, potrebno je pozvati metodu dijagnostika.obradjivac_ceka a nakon toga obrađivač treba da pređe u stanje čekanja, dok se procesor ne oslobodi.
    // Kada obrađivač uspe da zauzme procesor, pre obrade prekida treba da se pozove dijagnostika.obradjivac_izvrsava.
    // Nakon što je obrađivač završio obradu, potrebno je pozvati dijagnostika.obradjivac_zavrsio.
    void prekini()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        prekid = true;
        while (!slobodan_processor)
        {
            dijagnostika.obradjivac_ceka();
            cv_ui.wait(l);
        }
        slobodan_processor = false;
        dijagnostika.obradjivac_izvrsava();
        l.unlock();
        this_thread::sleep_for(chrono::milliseconds(300));
        l.lock();
        slobodan_processor = true;
        prekid = false;
        dijagnostika.obradjivac_zavrsio();
        cv_process.notify_one();
    }
};

#endif // PROCESOR_H_INCLUDED
