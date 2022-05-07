#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include "dijagnostika.h"
#include "naredba.h"
#include "povratna_vrednost.h"
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class Program
{
private:
    Dijagnostika &dijagnostika;
    mutex m;
    condition_variable cv, cv_vm;
    int ram_slobodno;
    int vm_kolicina;
    int vm_zauzeta;
    bool kraj;

public:
    Program(Dijagnostika &d, int kapacitet) : dijagnostika(d)
    {
        // Proširiti po potrebi ...
        ram_slobodno = kapacitet;
        vm_kolicina = 0;
        kraj = false;
    }

    Dijagnostika &getDijagnostika()
    {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje programa, kako bi zauzela potrebnu memoriju
    //
    // naredba - naredba koja se izvršava (naredba kojom se zauzima memorija)
    // Vraća instancu klase Povratna_vrednost koja sadrži opis zauzeća memorije NAKON izvršenja naredbe.
    Povratna_vrednost izvrsi_naredbu(Naredba naredba)
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        int kolicina_mem = naredba.kolicina_memorije;
        if (naredba.tip == "repeat")
        {
            kolicina_mem *= naredba.ponavljanja;
        }
        while (ram_slobodno < kolicina_mem)
        {
            vm_kolicina = kolicina_mem - ram_slobodno;
            cv_vm.notify_one();
            cv.wait(l);
        }
        Povratna_vrednost p;
        ram_slobodno -= naredba.kolicina_memorije;
        p.slobodno_ram = ram_slobodno;
        p.zauzeto_u_virtuelnoj = vm_zauzeta;
        return p;
    }

    // Metoda koju poziva nit koja simulira deo operativnog sistema koji se bavi zauzimanjem virtuelne memorije kako bi se zauzela određena količina VM
    // Potrebnu količinu VM (kao i bilo koju drugu neophodnu informaciju) preneti u nekom izabranom polju klase.
    int zauzmi_virtuelnu_memoriju()
    {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while(vm_kolicina != 0  && !kraj){
            cv_vm.wait(l);
        }
        while(kraj) this_thread::sleep_for(chrono::seconds(1));
        vm_zauzeta = vm_kolicina;
        vm_kolicina=  0;
        cv.notify_one();
        return vm_zauzeta;

    }

    void Kraj(){
        kraj = true;
        cv_vm.notify_one();
    }
};

#endif // PROGRAM_H_INCLUDED
