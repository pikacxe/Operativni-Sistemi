/*
Napraviti konkurentni program koji modeluje klasu brojača. Interfejs klase sadrži sledeće metode:

class Brojac {
   public:
      void inc();
      void dec();
      friend ostream& operator<<(ostream& , Brojac& );
};

Metode inc i dec povećavaću i smanjuju vrednost brojača respektivno. Operator << služi za ispis brojača na ekran.

Klasa treba da zaštiti konzistentnost brojača u datim metodama uz pomoć klasa mutex i unique_lock.

Kreirati 1 globalni objekat brojača kome će pristupati 2 niti.

Kreirati 2 niti pri čemu jedna nit poziva metodu uvećavanja brojača 1 000 000 puta a druga metodu smanjivanja brojača 1000000 puta.
Na kraju programa ispisati konačnu vrednost brojača nakon uvećavanja i smanjivanja.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const int ITERACIJA = 1'000'000;

class Brojac
{
private:
   int i;
   mutex m;

public:
   Brojac() : i(0) {}
   void inc()
   {
      unique_lock<mutex> l(m);
      ++i;
   }
   void dec()
   {
      unique_lock<mutex> l(m);
      --i;
   }
   friend ostream &operator<<(ostream &out, Brojac &b)
   {
      unique_lock<mutex> l(b.m);
      cout << "Vrednost brojaca: " << b.i << endl;
      return out;
   }
};

Brojac br;

void increment()
{
   for (int i = 0; i < ITERACIJA; ++i)
   {
      br.inc();
   }
}

void decrement()
{

   for (int i = 0; i < ITERACIJA; ++i)
   {
      br.dec();
   }
}

int main()
{
   thread t1(increment), t2(decrement);
   t1.join();
   t2.join();
   cout << br << endl;
   return 0;
}
