/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok. 
Nakon toga korisnik unosi svoju visinu. Na kraju u funkciji se ispisuje uneta visina.

Sprečiti štetno preplitanje na terminalu korišćenjem klase mutex.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/
/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu.
Na kraju u funkciji se ispisuje uneta visina.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

void f()
{
    double visina;
    m.lock();
    cout << "Unesite visinu:" << endl;
    cin >> visina;
    cout << "Uneta visina je: " << visina << "cm" << endl;
    cout << endl;
    m.unlock();
}

int main()
{
    thread t1(f), t2(f);
    t1.join();
    t2.join();

    return 0;
}
