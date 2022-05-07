#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

#include <iostream>
#include <string>
#include <mutex>

#include "pol.h"

using namespace std;

class Student {
private:
    mutex m;
    string naziv_pola(Pol p) {
        return p == MUSKI ? string("muski") : string("zenski");
    }

public:
    // rbr - Redni broj studenta
    // p   - Pol studenta
    void ceka(int rbr, Pol p) {
        unique_lock<mutex> l(m);
        cout << "Student sa rednim brojem " << rbr << " (pol: " << naziv_pola(p) << ") ceka." << endl;
    }

    // rbr - Redni broj studenta
    // p   - Pol studenta
    void ulazi(int rbr, Pol p) {
        unique_lock<mutex> l(m);
        cout << "Student sa rednim brojem " << rbr << " (pol: " << naziv_pola(p) << ") ulazi u ordinaciju." << endl;
    }
};

#endif // STUDENT_H_INCLUDED
