/*
Napraviti konkurentni program koji pokreće više niti u petlji.
Svakoj niti proslediti njen redni broj prilikom kreiranja i svaka nit treba da ispiše sopstveni redni broj u okviru tela niti.
*/

#include <iostream>
#include <thread>

using namespace std;

void nit(int i){
    cout << i << endl;
}

int main()
{
    const int n = 10;
    thread niti[n];
    for(int i = 0; i < n; ++i){
        niti[i] = thread(nit,i+1);
    }
    for(int i = 0; i < n; ++i){
        niti[i].join();
    }

    return 0;
}
