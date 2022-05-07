#ifndef STAZA_H_INCLUDED
#define STAZA_H_INCLUDED

#include "povratna.h"
#include "takmicar.h"
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <random>

using namespace std;
using namespace chrono;

class Atletska_staza
{
private:
    Takmicar &takmicar;
    mutex m;
    condition_variable cs, cb;
    int br_skakaca;
    int br_bacaca;
    bool zauzeta;
    steady_clock::time_point pocetak;

public:
    Atletska_staza(Takmicar &tak) : takmicar(tak)
    {
        // Prosiriti po potrebi ...
        br_skakaca = 0;
        br_bacaca = 0;
        zauzeta = false;
    }

    // Metoda koju poziva nit koja simulira skakaca kako bi takmicar obavio skok.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.skakac_ceka kada skakac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.skakac_skace kada skakac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.skakac_zavrsio kada je skakac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost skaci(int rbr)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        pocetak = steady_clock::now();
        while (zauzeta)
        {
            takmicar.skakac_ceka(rbr);
            br_skakaca++;
            cout << "Skakac: " << br_skakaca << endl;
            cs.wait(l);
        }
        br_skakaca--;
        cout << "Skakac: " << br_skakaca << endl;
        zauzeta = true;
        takmicar.skakac_skace(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(1));
        l.lock();
        zauzeta = false;
        Povratna_vrednost pv;
        pv.rezultat = rand() % 10;
        pv.trajanje = steady_clock::now() - pocetak;
        takmicar.skakac_zavrsio(rbr, pv);
        if (br_bacaca == 0)
        {
            cs.notify_one();
        }
        else
        {
            cb.notify_one();
        }
        return pv;
    }

    // Metoda koju poziva nit koja simulira bacaca kako bi takmicar obavio bacanje.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.bacac_ceka kada bacac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.bacac_skace kada bacac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.bacac_zavrsio kada je bacac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost baciKoplje(int rbr)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        pocetak = steady_clock::now();
        while (zauzeta)
        {
            takmicar.bacac_ceka(rbr);
            br_bacaca++;
            cout << "Bacac: " << br_bacaca << endl;
            cb.wait(l);
        }
        br_bacaca--;
        cout << "Bacac: " << br_bacaca << endl;
        zauzeta = true;
        takmicar.bacac_baca(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(2));
        l.lock();
        zauzeta = false;
        Povratna_vrednost pv;
        pv.rezultat = rand() % 100;
        pv.trajanje = steady_clock::now() - pocetak;
        takmicar.bacac_zavrsio(rbr, pv);
        if (br_skakaca == 0)
        {
            cb.notify_one();
        }
        else
        {
            cs.notify_one();
        }
        cs.notify_one();
        return pv;
    }
};

#endif // STAZA_H_INCLUDED
