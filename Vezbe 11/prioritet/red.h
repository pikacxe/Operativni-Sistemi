#ifndef DEF_RED
#define RED

#include "cv_hrono.h"
#include <deque>

class Red
{
private:
    int prioritet;
    cv_hrono *cv;
    deque<int> cekaju;

public:
    Red(int p) : prioritet(p)
    {
        cv = new cv_hrono;
    }
    void pocisti()
    {
        delete cv;
    }
    void dodaj_u_red(unique_lock<mutex> &l, int id_procesa)
    {
        cekaju.push_back(id_procesa);
        cv->wait(l);
    }
    bool prazan()
    {
        return cekaju.empty();
    }
    int get_prioritet() const { return prioritet; }
    int preuzmi_iz_reda()
    {
        int id = cekaju.front();
        cekaju.pop_front();
        cv->notify_one();
        return id;
    }
};

#endif