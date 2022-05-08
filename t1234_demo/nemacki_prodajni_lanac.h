#ifndef NEMACKI_PRODAJNI_LANAC_H_INCLUDED
#define NEMACKI_PRODAJNI_LANAC_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <random>

#include "kupac.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <random>

using namespace std;
using namespace chrono;

struct Kasa{
    int red;
    int id;
    bool aktivna;
    condition_variable cv;
    Kasa(int i): red(0), id(i), aktivna(false){}
};

class Eksamarket {
    private:
        // Dodaj B
        Kupac& k;
        mutex m;
        vector<Kasa> kase;

    public:
        Eksamarket(Kupac &k, int broj_kasa): k(k) {
            // Dopuni B po potrebi
            for(int i = 0;i<broj_kasa;i++){
                kase.push_back(Kasa(i));
            }
        }
        ~Eksamarket() {
            // Dopuni B po potrebi
        }

	/**
	Kupac dolazi na kase i zauzima prvu slobodnu kasu koja je aktivna. Ako ne postoje takve kase odlazi na cekanje u najkraci red cekanja aktivne kase.
	
	pozvati Kupac.prilazi_kasama na pocetku
	pozvati Kupac.ceka ako kupac odlazi na cekanje na nekoj kasi
	pozvati Kupac.kupuje kada kupac zauzme kasu i pocne sa kupovinom
	pozvati Kupac.odlazi kada kupac zavrsi kupovinu
	@param id - r. br. kupca
	*/
        void kupovina(int id) {
            // Implementiraj B kupac
            unique_lock<mutex> l(m);
            auto it = kase.begin();
            for(; it != kase.end(); ++it){
                if(it->aktivna){
                    break;
                }
            }


        }

	/**
	Na svakih 1s treba jednu aktivnu kasu deaktivirati i jednu neaktivnu kasu aktivirati.
	
	pozvati Kupac.smena_kasa nakon sto se po jedna kasa aktivira i deaktivira
	*/
        void smena_kasa() {
            // Implementiraj B smena
            while(true){
                int id = rand() % kase.size();
                kase.at(id).aktivna = false;
                id = rand() % kase.size();
                kase.at(id).aktivna = true;
                this_thread::sleep_for(seconds(1));
            }
        }
};


#endif // NEMACKI_PRODAJNI_LANAC_H_INCLUDED
