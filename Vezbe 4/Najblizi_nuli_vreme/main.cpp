#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

using namespace std;
using namespace chrono;

typedef vector<double>::const_iterator cit;

struct vreme
{
    system_clock::time_point pocetak;
    system_clock::time_point kraj;
};
void f(cit pocetak, cit kraj, double &min, struct vreme *v)
{
    v->pocetak = system_clock::now();
    cit it;
    min = *pocetak;
    for (it = pocetak + 1; it != kraj; ++it)
    {
        if (*it < min)
        {
            min = *it;
        }
    }
    v->kraj = system_clock::now();
}

const int BROJ_NITI = 3;
const int DUZINA = 900000;

int main()
{
    int SEGMENT = DUZINA / BROJ_NITI;

    srand(time(NULL));

    vector<double> v(DUZINA);
    for (int i = 0; i < DUZINA; i++)
    {
        v[i] = rand();
    }
    double minimumi[BROJ_NITI];
    struct vreme vremena[BROJ_NITI];

    thread niti[BROJ_NITI];

    cit it;
    for (int i = 0; i < BROJ_NITI; i++)
    {
        niti[i] = thread(f, v.begin() + i * SEGMENT, v.begin() + (1 + i) * SEGMENT, ref(minimumi[i]), &vremena[i]);
    }
    for (int i = 0; i < BROJ_NITI; ++i)
    {
        niti[i].join();
    }
    double najblizi_nuli = minimumi[0];
    for (int i = 0; i < BROJ_NITI; i++)
    {
        if (abs(najblizi_nuli) > abs(minimumi[i]))
        {
            najblizi_nuli = minimumi[i];
        }
        cout << "Minimum[" << i << "] = " << minimumi[i] << endl;
    }
    cout << "Najblizi nuli: " << najblizi_nuli << endl;
    for (int i = 0; i < BROJ_NITI; ++i)
    {
        duration<double, ratio<1, 1000>> interval = vremena[i].kraj - vremena[i].pocetak;
        cout << "Vreme izvrsenja niti[" << i << "] = " << interval.count() << endl;
    }
    return 0;
}