#ifndef NEMACKI_PRODAJNI_LANAC_H_INCLUDED
#define NEMACKI_PRODAJNI_LANAC_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <random>

#include "kupac.h"

using namespace std;
using namespace chrono;


class Eksamarket {
    private:
        // Dodaj B
    public:
        Eksamarket(Kupac &k, int broj_kasa) {
            // Dopuni B po potrebi
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

        }

	/**
	Na svakih 1s treba jednu aktivnu kasu deaktivirati i jednu neaktivnu kasu aktivirati.
	
	pozvati Kupac.smena_kasa nakon sto se po jedna kasa aktivira i deaktivira
	*/
        void smena_kasa() {
            // Implementiraj B smena
        }
};


#endif // NEMACKI_PRODAJNI_LANAC_H_INCLUDED
