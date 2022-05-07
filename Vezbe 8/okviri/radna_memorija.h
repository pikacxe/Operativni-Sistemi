#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <vector>


using namespace std;

class Radna_memorija {
private:
    Dijagnostika& dijagnostika;
    int br_slobodnih_okvira;
    vector<int> okviri;
    mutex m;
    condition_variable cv;

public:
    // dijagnostika  - referenca na instancu klase Dijagnostika
	// ukupno_okvira - ukupan broj okvira u memoriji
    Radna_memorija(Dijagnostika& d, int ukupno_okvira) : dijagnostika(d) {
        // Proširiti po potrebi ...
        br_slobodnih_okvira = ukupno_okvira;
        okviri.resize(ukupno_okvira, -1);

    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi pokušala da učita proces u radnu memoriju
    //
    // broj_stranica - ukupan broj stranica koje proces sadrži
    // id_procesa    - identifikator procesa
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno memorije (broj stranica u procesu je veći od broja raspoloživih okvira memorije), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_izvrsava. Nakon toga,
    // kada proces zauzme okvire radne memorije, potrebno je pozvati dijagnostika.ispisi_okvire kako bi se prikazalo trenutno zauzece svih okvira (podrazumeva se da zelimo da prikazemo sliku svih okvira, tako da ce se videti i okviri koje su zauzeli drugi procesi).
    void ucitaj(int broj_stranica, int id_procesa) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(br_slobodnih_okvira < broj_stranica){
            dijagnostika.proces_ceka(id_procesa);
            cv.wait(l);
        }
        int trenutno_zauzetih = 0;
        for(auto it = okviri.begin(); it != okviri.end(); ++it){
            if((*it) == -1){
                (*it) = id_procesa;
                br_slobodnih_okvira--;
                trenutno_zauzetih++;
            }
            if(trenutno_zauzetih == broj_stranica){
                break;
            }
        }
        dijagnostika.proces_se_izvrsava(id_procesa);
        dijagnostika.ispisi_okvire(okviri.begin(), okviri.end());
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi oslobodila radnu memoriju koju je koristila tokom izvršenja
    //
    // id_procesa - identifikator procesa koji oslobađa memoriju
    void oslobodi(int id_procesa) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        for(auto it = okviri.begin(); it != okviri.end(); ++it){
            if((*it) == id_procesa){
                (*it) = -1;
                br_slobodnih_okvira++;
            }
        }
        dijagnostika.ispisi_okvire(okviri.begin(), okviri.end());
        cv.notify_all();
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED
