#ifndef MAGISTRALA_H_INCLUDED
#define MAGISTRALA_H_INCLUDED

#include "dijagnostika.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>


using namespace std;
using namespace chrono;

// Klasa magistrale (sabirnice). Glavna deljena promenljiva za sinhronizaciju pristupa procesora memoriji i dma kontroleru.
class Magistrala {
public :
    // Stanja sabirnice - slobodna, čitanje iz memorije, pisanje u memoriju, dma transfer. Primetiti sličnost sa stanjima iz zadatka "multiprocesor":
    enum Stanje { SLOBODNA, MEM_CITAJ, MEM_PISI, DMA};
    // Struktura u koju se beleže parametri DMA transfera. Odakle, kome i koliko bajtova se čita ili piše.
    struct DMA_transfer {
        int odakle;
        int koliko;
        int kome;
    };
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv, cv_obrada;
    bool gotovo, dma_obradjen;
    Memorija& mem;
    Stanje stanje;
    DMA_transfer trenutni;

public:


    Magistrala(Dijagnostika& d, Memorija& mem) : dijagnostika(d), mem(mem) {
        // Proširiti po potrebi ...
        gotovo = false;
        stanje = SLOBODNA;
        dma_obradjen = false;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    char citaj_memoriju(int adresa) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA){
            cv.wait(l);
        }
        stanje = MEM_CITAJ;
        l.unlock();
        this_thread::sleep_for(milliseconds(700));
        l.lock();
        stanje = SLOBODNA;
        cv.notify_one();
        return mem.citaj(adresa);
    }

    void pisi_u_memoriju(int adresa, char vrednost) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA){
            cv.wait(l);
        }
        stanje = MEM_PISI;
        l.unlock();
        this_thread::sleep_for(milliseconds(700));
        l.lock();
        mem.pisi(adresa, vrednost);
        stanje = SLOBODNA;
        cv.notify_one();
    }

    void dma(DMA_transfer transfer) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA){
            cv.wait(l);
        }
        stanje = DMA;
        l.unlock();
        this_thread::sleep_for(milliseconds(700));
        l.lock();
        trenutni = transfer;
        dma_obradjen = true;
        cv_obrada.notify_one();
    }

    DMA_transfer okidac_dma_kontrolera() {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(!dma_obradjen && !gotovo){
            cv_obrada.wait(l);
        }
        if(gotovo) return (DMA_transfer){-1,0,0};
        for(int i = 0; i< trenutni.koliko; i++){
            mem.pisi(trenutni.kome+i, mem.citaj(trenutni.odakle+i));
        }
        stanje = SLOBODNA;
        dma_obradjen = false;
        cv.notify_one();
        return trenutni;
    }
    
    void zavrsi() {
        // Implementirati ...
        gotovo = true;
        cv_obrada.notify_one();
    }
};

#endif // MAGISTRALA_H_INCLUDED
