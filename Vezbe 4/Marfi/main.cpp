#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

const int UKUPNO_KASA = 3;
const int MAX_MILISEKUNDI_PO_KUPCU = 100;

void kasa(int broj_kupaca, duration<double, milli> &vreme_na_kasi)
{
    system_clock::time_point pocetak = system_clock::now();
    for(int i = 0;i<broj_kupaca;i++){
        duration<double,milli> trajanje(rand() % MAX_MILISEKUNDI_PO_KUPCU + 1);
        this_thread::sleep_for(trajanje);
    }
    system_clock::time_point kraj = system_clock::now();
    vreme_na_kasi = (pocetak - kraj);
}

int main()
{
    srand(time(NULL));

    int moja_kasa;
    cout << "Unesite kasu na kojoj cete stati u red (0, 1 ili 2):" << endl;
    cin >> moja_kasa;

    duration<double, milli> vremena[UKUPNO_KASA];

    thread niti[UKUPNO_KASA];
    for (int i = 0; i < UKUPNO_KASA; ++i)
    {
        niti[i] = thread(kasa, rand() % 30 + 1, ref(vremena[i]));
    }

    for(int i = 0; i< UKUPNO_KASA; ++i){
        niti[i].join();
    }
    double najduze_vreme = vremena[0].count();
    int index = 0;
    for(int i = 1; i< UKUPNO_KASA; ++i){
        if(najduze_vreme < vremena[i].count()){
            najduze_vreme = vremena[i].count();
            index = i;
        }
    }
    if(index == moja_kasa){
        cout << "Odabrali ste kasu na kojoj je vreme kupovine bilo najduze!" << endl;
    }
    else{
        cout << "Niste odabrali ste kasu na kojoj je vreme kupovine bilo najduze!" << endl;
    }
}