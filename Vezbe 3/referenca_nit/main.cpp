#include <iostream>
#include <thread>

using namespace std;

void f(int a, int b){
    cout << "Rezultat je: " << a+b << endl;
}

int main(){
    int a = 5;
    int b = 10;
    thread t(f,a,b);
    t.join();
    return 0;
}