#ifndef TENISKI_KLUB_H_INCLUDED
#define TENISKI_KLUB_H_INCLUDED

#include "podaci.h"
#include "mec.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

#define MAX_TERENA 30

using namespace std;

class Teniski_klub {                             //Klasa deljenog resursa. Pravi se maksimalno 30 terena, ali ce u realnosti biti samo 3 terena.
private:
    Mec& mec;
    mutex m;
    condition_variable cv;
    vector<int> slobodni_tereni;
public:
    Teniski_klub(Mec& m, int ukupno_terena) : mec(m) {
        // Prosiriti po potrebi ...
        if(ukupno_terena > MAX_TERENA) {
            cout << "Ukupno terena je veci od maksimalnog broja terena." << endl;
            exit(1);
        }
        slobodni_tereni.resize(ukupno_terena,0);
    }

    // Metoda koju poziva nit koja simulira mec kako bi izvrsila teniski mec.
    // Metoda je blokirajuca - ako je zeljeni teren zauzet, izvrsenje ce blokirati dok se on ne oslobodi.
    //
    // broj_meca - Redni broj meca
    // na_terenu - Indeks terena na kojem mec treba da se odigra
    //
    // Potrebno je pozvati metodu mec.ceka kada je izabrani teren zauzet i mec mora da ceka.
    // Potrebno je pozvati metodu mec.pocinje kada se isprazni izabrani teren i mec moze da pocne.
    // Potrebno je pozvati metodu mec.zavrsen kada se mec zavrsi i mogu se proslediti izmereni vremenski intervali.
    void odigraj_mec(int broj_meca, int na_terenu) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        Podaci p;
        p.dosao = system_clock::now();
        while(slobodni_tereni[na_terenu] == 1){
            mec.ceka(broj_meca,na_terenu);
            cv.wait(l);
        }
        p.pocetak = system_clock::now();
        slobodni_tereni[na_terenu] = 1;
        mec.pocinje(broj_meca,na_terenu);
        l.unlock();
        this_thread::sleep_for(seconds(rand()%5+1));
        l.lock();
        slobodni_tereni[na_terenu] = 0;
        p.trajanje = system_clock::now() - p.pocetak;
        mec.zavrsen(broj_meca,p);
        cv.notify_all();
    }
};

#endif // TENISKI_KLUB_H_INCLUDED
