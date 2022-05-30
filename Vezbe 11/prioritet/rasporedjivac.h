#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include "red.h"

using namespace std;
using namespace chrono;

class Rasporedjivac
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    vector<Red> redovi;
    int aktivan;

public:
    Rasporedjivac(Dijagnostika &d, int broj_nivoa_prioriteta) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        aktivan = -1;
        for (int i = 0; i < broj_nivoa_prioriteta; ++i)
        {
            redovi.push_back(Red(i));
        }
    }
    ~Rasporedjivac(){
        for(auto red:redovi){
            red.pocisti();
        }
    }

    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // broj_naredbi - ukupan broj naredbi koje proces treba da izvrši
    // prioritet    - prioritet procesa
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka, a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
    void izvrsi(int id_procesa, int broj_naredbi, int prioritet)
    {
        // Implementirati ...
        for(int i = 0; i< broj_naredbi;++i){
            unique_lock<mutex> l(m);
            if(aktivan == -1){
                aktivan = id_procesa;
            }
            while(aktivan != id_procesa){
                dijagnostika.proces_ceka(id_procesa);
                redovi[prioritet].dodaj_u_red(l, id_procesa);
            }
            l.unlock();
            this_thread:: sleep_for(chrono::milliseconds(300));
            l.lock();
            int obavesti_red = -1;
            for(auto red: redovi){
                if(!red.prazan()){
                    obavesti_red = red.get_prioritet();
                    break;
                }
            }
            if(obavesti_red == -1) aktivan = -1;
            else {
                if(obavesti_red > prioritet && i < broj_naredbi - 1){
                    aktivan = id_procesa;
                } else{
                    aktivan = redovi[obavesti_red].preuzmi_iz_reda();
                }
            }
            dijagnostika.izvrsio_naredbu(id_procesa,i);
        }
    }
};

#endif // RASPOREDJIVAC_H_INCLUDED
