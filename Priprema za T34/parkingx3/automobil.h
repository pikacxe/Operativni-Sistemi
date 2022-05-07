#ifndef AUTOMOBIL_H_INCLUDED
#define AUTOMOBIL_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Automobil {
private:
    mutex m;
public:
    // rbr  - Redni broj automobila
    // ulaz - Ulaz na koji automobil pokusava da udje
    void ceka(int rbr, int ulaz) {
        unique_lock<mutex> l(m);
        cout << "Automobil "<< rbr <<" ceka kako bi usao na parking, na ulazu "<< ulaz <<endl;
    }

    // rbr  - Redni broj automobila
    void zauzima(int rbr) {
        unique_lock<mutex> l(m);
        cout << "Automobil " << rbr <<" je zauzeo parking." << endl;
    }

    // rbr  - Redni broj automobila
    void napusta(int rbr) {
        unique_lock<mutex> l(m);
        cout << "Automobil "<< rbr << " je napustio parking." << endl;
    }
};

#endif // AUTOMOBIL_H_INCLUDED
