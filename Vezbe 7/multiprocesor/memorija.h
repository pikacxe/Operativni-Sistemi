#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

enum Stanje {SLOBODAN, CITANJE, UPIS };

class Memorija {
private:
    Dijagnostika& dijagnostika;

public:
    Memorija(Dijagnostika& d, int bajtova) : dijagnostika(d) {
        // Proširiti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila čitanje iz nje
    //
    // rbp    - Redni broj procesa
    // adresa - Lokacija sa koje se čita vrednost prethodno upisana u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_citanje kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_procitao onda kada je ostvaren pristup memoriji i kada je vrednost učitana iz nje.
    char citaj(int rbp, int adresa) {
        // Implementirati ...
    }

    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila upis u nju
    //
    // rbp      - Redni broj procesa
    // adresa   - Lokacija u koju se upisuje nova vrednost
    // vrednost - Vrednost koja se upisuje u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_upis kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_upisao onda kada je ostvaren pristup memoriji i kada je vrednost upisana u nju.
    void pisi(int rbp, int adresa, char vrednost) {
        // Implementirati ...
    }
};

#endif // MEMORIJA_H_INCLUDED
