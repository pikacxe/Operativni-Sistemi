#ifndef BANKA_H_INCLUDED
#define BANKA_H_INCLUDED

#include "valuta.h"
#include "kredit.h"

#include <mutex>
#include <condition_variable>

using namespace std;

class Banka {
private:
    Kredit& kredit;
    int dsaldo;
    int esaldo;
    mutex m;
    condition_variable cvd;
    condition_variable cve;
public:

    Banka(Kredit& kr, int inicijalni_dsaldo, int inicijalni_esaldo)
        : kredit(kr) {
        // Prosiriti po potrebi ...
        dsaldo = inicijalni_dsaldo;
        esaldo = inicijalni_esaldo;
    }

    // Metoda koju poziva nit koja simulira klijenta banke, kada klijent zatraži kredit od banke.
    // Ukoliko banka nema odgovarajuća sredstva kako bi omogućila kredit, metoda će blokirati dok se ta sredstva ne oslobode.
    //
    // rbr - Redni broj klijenta
    // svota - Novčani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novčani iznos
    //
    // Potrebno je pozvati metodu kredit.ceka kada nema sredstava odgovarajuće valute da se odobri kredit.
    // Potrebno je pozvati metodu kredit.dobio kada se kredit realizuje.
    void uzmi_kredit(int rbr, int svota, Valuta valuta) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        if(valuta == Valuta::DINAR){
            while(dsaldo < svota){
                kredit.ceka(rbr,svota,valuta);
                cvd.wait(l);
            }
            dsaldo -= svota;
            kredit.dobio(rbr,svota,dsaldo,valuta);
        }
        else if(valuta == Valuta::EURO){
            while(esaldo < svota){
                kredit.ceka(rbr,svota,valuta);
                cve.wait(l);
            }
            esaldo -= svota;
            kredit.dobio(rbr,svota,esaldo,valuta);
        }


    }

    // Metoda koju poziva nit koja simulira klijenta banke, kada klijent vrati kredit koji je prethodno uzeo od banke.
    //
    // rbr - Redni broj klijenta
    // svota - Novčani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novčani iznos
    //
    // Potrebno je pozvati metodu kredit.vratio kada je kredit vraćen.
    void vrati_kredit(int rbr, int svota, Valuta valuta) {
        // Implementirati ...
        if(valuta == Valuta::DINAR){
            dsaldo +=svota;
            kredit.vratio(rbr,svota,dsaldo,valuta);
            cvd.notify_all();
        }
        else if(valuta == Valuta::EURO){
            esaldo +=svota;
            kredit.vratio(rbr,svota,esaldo,valuta);
            cve.notify_all();
        }
    }
};

#endif // BANKA_H_INCLUDED
