/*
Izmeniti program data_race i realizovati zaštitu pristupa brojaču uz pomoć klase mutex i njenih operacija lock() i unlock().
Primetiti koliko sada traje izvršavanje programa.
*/


/*
Kreirati globalnu celobrojnu promeljivu brojač. Nakon toga kreirati 2 funkcije inkrement i dekrement koje povećavaju i smanjuju dati brojač ITERACIJA puta.
 ITERACIJA je konstanta koja predstavlja proizvoljno velik broj (npr. 100.000.000). 

Kreirati jednu nit od funkcije inkrement i jednu nit od funkcije dekrement.

Nakon završetka rada niti ispisati vrednosti brojača. Da li je data vrednost očekivana? 
*/
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;


mutex m;
int brojac = 0;
const int ITERACIJA = 100'000'000;


void increment(){
    for(int i= 0;i<ITERACIJA;++i){
        m.lock();
        brojac++;
        m.unlock();
    }
}

void decrement(){
    for(int i= 0;i<ITERACIJA;++i){
        m.lock();
        brojac--;
        m.unlock();
    }
}


int main(){
    thread t1(increment), t2(decrement);
    t1.join();
    t2.join();
    cout << brojac << endl;
    return 0;
}