#ifndef RC_H_INCLUDED
#define RC_H_INCLUDED

#include "student.h"
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

#define MAX 50 //maksimalni broj racunara u ucionici

class RC {
private:
    Student& student;
    mutex m;
    condition_variable cv;
    vector<int> slobodni;
    int br_slobodnih;
    
public:
	RC(Student& st, int br) : student(st) {
        // Prosiriti po potrebi ...
        slobodni.resize(br, 0);
        br_slobodnih = br; 
    }

    // Metoda koju poziva nit koja simulira studenta kako bi student zauzeo mesto za racunarom.
    // Ova metoda je blokirajuca - ako nema slobodnih mesta, ona ce cekati dok se neko ne oslobodi.
    //
    // rbr - Redni broj studenta
    //
    // Potrebno je pozvati metodu student.ceka kada su racunari zauzeti i student mora da ceka.
    // Potrebno je pozvati metodu student.zauzeo kada student zauzme racunar.
    int zauzmi(int rbr) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        while( br_slobodnih == 0) {
            student.ceka(rbr);
            cv.wait(l);
        }
        br_slobodnih--;
        int rac_id = 0;
        for(int i = 0; i < slobodni.size(); i++) {
            if(slobodni[i] == 0) {
                rac_id = i;
                slobodni[i] = 1;
                break;
            }
        }
        student.zauzeo(rbr, rac_id);
        return rac_id;
    }

    // Metoda koju poziva nit koja simulira studenta kako bi oslobodio racunar koji je prethodno zauzeo.
    //
    // rbr         - Redni broj studenta
    // id_racunara - Redni broj racunara kojeg student oslobadja (prethodno je zauzeo taj racunar)
    //
    // Potrebno je pozvati metodu student.oslobodio kada student oslobodi racunar.
    void oslobodi(int rbr, int id_racunara) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        br_slobodnih++;
        slobodni[id_racunara] = 0;
        student.oslobodio(rbr, id_racunara);
        cv.notify_one();
    }
};

#endif // RC_H_INCLUDED
