#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>

using namespace std;
using namespace chrono;

class Radna_memorija
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    condition_variable cv_process;
    condition_variable cv_mem;
    bool poziv, kraj;
    vector<int> okviri;


public:
    Radna_memorija(Dijagnostika &d, int ukupno_lokacija) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        okviri.resize(ukupno_lokacija, -1);
        poziv = false;
        kraj = false;
    }

    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi proces zauzeo potrebnu memoriju i krenuo sa radom
    //
    // id_procesa          - identifikator procesa
    // br_lokacija_procesa - ukupan broj lokacija memorije koje su neophodne za izvršenje procesa
    // trajanje            - koliko dugo se proces izvršava (u sekundama)
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno slobodne memorije (moraju se zauzeti uzastopni okviri u memoriji!), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_zauzeo_okvire.
    // Kada se proces završi, potrebno je pozvati metodu dijagnostika.proces_zavrsio.
    // Metodu dijagnostika.ispisi_memoriju potrebno je pozvati u sledećim momentima: nakon što proces zauzme memoriju i nakon što proces oslobodi memoriju.
    void koristi(int id_procesa, int br_lokacija_procesa, int trajanje)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (count(okviri.begin(), okviri.end(), -1) < br_lokacija_procesa)
        {
            dijagnostika.proces_ceka(id_procesa);
            cv_process.wait(l);
        }
        poziv = false;
        vector<int>::iterator it = find(okviri.begin(), okviri.end(), -1);
        for (auto ita = it; ita != it + br_lokacija_procesa; ++ita)
        {
            (*ita) = id_procesa;
        }
        int index = 0;
        for (auto ita = okviri.begin(); ita != it; ++ita)
        {
            index++;
        }

        dijagnostika.proces_zauzeo_okvire(id_procesa, index, index + br_lokacija_procesa);
        dijagnostika.ispisi_memoriju(okviri.begin(), okviri.end());
        l.unlock();
        this_thread::sleep_for(chrono::seconds(trajanje));
        l.lock();
        it = find(okviri.begin(), okviri.end(), id_procesa);
        for (auto ita = it; ita != it + br_lokacija_procesa; ++ita)
        {
            (*ita) = -1;
        }
        poziv = true;
        dijagnostika.proces_zavrsio(id_procesa);
        cv_mem.notify_one();
    }

    // Metoda koju poziva nit operativnog sistema koji se bavi sažimanjem memorije
    //
    // Nakon što je sažimanje obavljeno potrebno je pozvati metodu dijagnostika.sazimanje_obavljeno a nakon toga i metodu dijagnostika.ispisi_memoriju kako bi se prikazali efekti sažimanja memorije.
    void sazimanje()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while (poziv == false && !kraj)
        {
            cv_mem.wait(l);
        }
        while(kraj) this_thread::sleep_for(chrono::seconds(1));
        auto it = find(okviri.begin(), okviri.end(), -1);
        for(auto ita = it; ita != okviri.end(); ita++){
            if(*ita != -1){
                int temp = *ita;
                (*it) = *ita;
                *ita = -1;
                it++; 
            }
        }
        poziv = false;
        dijagnostika.sazimanje_obavljeno();
        dijagnostika.ispisi_memoriju(okviri.begin(), okviri.end());
        cv_process.notify_all();
        
    }

    void Kraj(){
        kraj = true;
        cv_mem.notify_one();
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED
