/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu.
Na kraju u funkciji se ispisuje uneta visina.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/

#include <iostream>
#include <thread>

using namespace std;

void f()
{
    double visina = 0;
    cout << "Unesite visinu:" << endl;
    cin >> visina;
    cout << "Uneta visina je: " << visina << "cm" << endl;
}

int main()
{
    thread t1(f), t2(f);
    t1.join();
    t2.join();

    return 0;
}
