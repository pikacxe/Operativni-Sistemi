#ifndef AMBULANTA_H_INCLUDED
#define AMBULANTA_H_INCLUDED

#include "student.h"
#include <mutex>
#include <condition_variable>

using namespace std;

class Studentska_ambulanta {
private:
    Student& student;
    mutex m;
    condition_variable cvm;
    condition_variable cvz;
    int slobodno_m;
    int slobodno_z;
public:
    Studentska_ambulanta(Student& st, int muskih, int zenskih) : student(st) {
        // Prosiriti po potrebi ...
        slobodno_m = muskih;
        slobodno_z = zenskih;
    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti kada student pokusava da udje u nju.
    // Ukoliko je ambulanta zauzeta, izvrsenje ove metode ce cekati da se ona oslobodi.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    //
    // Potrebno je pozvati metodu student.ceka kada student ne moze da stupi u ambulantu.
    // Potrebno je pozvati metodu student.ulazi kada student udje u ambulantu.
    void udji(int rbr, Pol p) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        if(p == Pol::MUSKI){
            while(slobodno_m == 0){
                student.ceka(rbr, p);
                cvm.wait(l);
            }
            student.ulazi(rbr,p);
            slobodno_m--;

        }
        else if(p == Pol::ZENSKI){
            while(slobodno_z == 0){
                student.ceka(rbr, p);
                cvz.wait(l);
            }
            student.ulazi(rbr,p);
            slobodno_z--;
        }
    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti nakon sto je student pregledan i izlazi iz nje.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    void izadji(int rbr, Pol p) {
        // Implementirati ...
        if(p == Pol::MUSKI){
            slobodno_m++;
            cvm.notify_all();

        }
        else if(p == Pol::ZENSKI){
            slobodno_z++;
            cvz.notify_all();
        }
    }
};

#endif // AMBULANTA_H_INCLUDED
