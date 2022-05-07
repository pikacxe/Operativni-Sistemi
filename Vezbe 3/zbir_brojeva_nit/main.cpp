/*
Napraviti program koji kreira jednu nit kojoj se prosleđuju dva cela broja a i b. U okviru niti sabrati brojeve i ispisati na ekran njihov zbir.
*/

#include <iostream>
#include <thread>

using namespace std;

void zbir_brojeva(int a, int b){
    cout << "Zbir prosledjenih brojeva je: " << a + b << endl;
}

int main()
{
    thread t(zbir_brojeva,4,5);
    t.join();

    return 0;
}
