#ifndef SKLADISTE_H_INCLUDED
#define SKLADISTE_H_INCLUDED

#include "kamion.h"
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class Skladiste
{
private:
    Kamion &kamion;
    mutex m;
    condition_variable normal;
    condition_variable boom;
    bool rampa1, rampa2;
    int zapaljivi_kamioni;

public:
    Skladiste(Kamion &k) : kamion(k)
    {
        // Prosiriti po potrebi ...
        zapaljivi_kamioni = 0;
        rampa1 = false;
        rampa2 = false;
    }

    // Metoda koju poziva nit koja simulira kretanje kamiona kada on pokusava da istovari robu.
    // Metoda je blokirajuca - ako su sve rampe za istovar zauzete, izvrsenje ce blokirati dok se neka ne oslobodi.
    //
    // rbr       - Redni broj kamiona
    // kolicina  - Kolicina robe koja se prevozi (u tonama)
    // zapaljivo - Ukazuje na to da li je roba koja se prevozi zapaljiva (takva roba ima prioritet pri istovaru!)
    //
    // Potrebno je pozvati metodu kamion.ceka kada su rampe zauzete i kamion mora da ceka.
    // Potrebno je pozvati metodu kamion.istovara kada zapocne istovar robe iz kamiona (primetiti da ta metoda zahteva da joj se prosledi indeks rampe na kojoj se obavlja istovar!)
    // Potrebno je pozvati metodu kamion.odlazi kada je kamion zavrsio istovar i odlazi.
    void istovari(int rbr, int kolicina, bool zapaljivo)
    {
        unique_lock<mutex> l(m);
        while (rampa1 == true && rampa2 == true)
        {
            kamion.ceka(rbr, kolicina, zapaljivo);
            if (zapaljivo)
            {
                zapaljivi_kamioni++;
                boom.wait(l);
            }
            else
            {
                normal.wait(l);
            }
        }
        int id_rampe = 0;
        if (rampa1 == false)
        {
            id_rampe = 0;
            rampa1 = true;
        }
        else if (rampa2 == false)
        {
            id_rampe = 1;
            rampa2 = true;
        }
        kamion.istovara(rbr, kolicina, zapaljivo, id_rampe);
        l.unlock();
        this_thread::sleep_for(chrono::milliseconds(kolicina * 1000));
        l.lock();
        if (id_rampe == 0)
        {
            rampa1 = false;
        }
        else if (id_rampe == 1)
        {
            rampa2 = false;
        }
        kamion.odlazi(rbr);
        if (zapaljivi_kamioni > 0)
        {
            zapaljivi_kamioni--;
            boom.notify_one();
        }
        else
        {
            normal.notify_one();
        }
    }
};

#endif // SKLADISTE_H_INCLUDED
