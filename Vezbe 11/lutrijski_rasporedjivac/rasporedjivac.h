#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"
#include "cv_tag.h"

using namespace std;

struct Proces {
	int id;
	int broj_naredbi;
	vector<int> tiketi;
	Proces(int i, int b, vector<int> t) : id(i), broj_naredbi(b), tiketi(t) {}
};

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    cv_tag cv;
    int aktivan;
    map<int, int> svi_tiketi;

public:
    Rasporedjivac(Dijagnostika& d) : dijagnostika(d) {
        // Proširiti po potrebi ...
        aktivan = 0;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // p - struktura koja sadrži ID procesa, broj naredbi koje treba da izvrši kao i sve tikete koji pripadaju tom procesu.
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka, a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(Proces p) {
        // Implementirati ...
        {
            unique_lock<mutex> l(m);
            for(auto tiket: p.tiketi){
                svi_tiketi[tiket] = p.id;
            }
        }
        for(int i = 0; i < p.broj_naredbi; ++i){
            unique_lock<mutex> l(m);
            if(aktivan != 0 && aktivan != p.id){
                dijagnostika.proces_ceka(p.id);
                cv.wait(l,p.id);
            }
            aktivan = p.id;
            l.unlock();
            this_thread:: sleep_for(chrono::milliseconds(300));
            l.lock();
            if(i == p.broj_naredbi -1){
               for(auto tiket: p.tiketi){
                   svi_tiketi.erase(tiket);
               }
            }
            if(!(svi_tiketi.empty())){
                int poz = rand()%svi_tiketi.size();
                auto it = svi_tiketi.begin();
                advance(it,poz);
                aktivan = it->second;
                cv.notify(aktivan);
                cout << "[LUTRIJA] Odabran tiket: " << it->first << " -> " << it->second << endl;
            }
            else {
                aktivan = 0;
            }
            dijagnostika.izvrsio_naredbu(p.id,i,aktivan);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED
