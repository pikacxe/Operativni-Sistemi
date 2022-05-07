#ifndef KREDIT_H_INCLUDED
#define KREDIT_H_INCLUDED

#include <iostream>
#include <mutex>

#include "valuta.h"

using namespace std;

class Kredit {
    mutex m;
public:
    // rbr - Redni broj klijenta
    // svota - Novčani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novčani iznos
    void ceka(int rbr, int svota, Valuta valuta) {
        unique_lock<mutex> l(m);
       cout << "Klijent: " << rbr << " mora da ceka kako bi dobio kredit od " << svota << " " << naziv_valute(valuta) << endl;
    }

    // rbr - Redni broj klijenta
    // svota - Novčani iznos koji je potreban klijentu
    // saldo - Ukupna raspoloživa sredstva u banci za traženu valutu (nakon odobrenja kredita)
    // valuta - Valuta u kojem je novčani iznos
    void dobio(int rbr, int svota, int saldo, Valuta valuta) {
        unique_lock<mutex> l(m);
        cout << "Klijent: " << rbr << " dobio " << svota << ", u banci ostalo: " << saldo << ", valuta: " << naziv_valute(valuta) << endl;
    }

    // rbr - Redni broj klijenta
    // svota - Novčani iznos koji je potreban klijentu
    // saldo - Ukupna raspoloživa sredstva u banci za traženu valutu (nakon što je iznos kredita vraćen)
    // valuta - Valuta u kojem je novčani iznos
    void vratio(int rbr, int svota, int saldo, Valuta valuta) {
        unique_lock<mutex> l(m);
        cout << "Klijent: " << rbr << " vratio " << svota << ", u banci ostalo: " << saldo << ", valuta: " << naziv_valute(valuta) << endl;
}
};

#endif // KREDIT_H_INCLUDED
