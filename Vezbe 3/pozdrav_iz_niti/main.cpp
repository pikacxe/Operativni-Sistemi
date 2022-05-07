/*
Napraviti program koji kreira jednu nit i u okviru niti ispisuje proizvoljnu rečenicu.
*/

#include <iostream>
#include <thread>

using namespace std;

void f1(){
    cout << "Hello World!" << endl;
}


int main()
{
    thread t(f1);
    t.join();
    return 0;
}
