/*
    Napisati konkurentni program koji koristi vise niti da generise i smesti u memoriju proste brojeve.
    Program se sastoji od tri faze, a, b, i c. Svaku od tih faza testira po jedna funkcija koja je
    makar parcijalno data.

    a) Napisati funkciju koja racuna proste brojeve pocevsi od neke vrednosti i koja se izvrsava dok ne
    generise specificiran broj prostih brojeva. Test pokrece ovu funkciju vise puta, paralelno i ispisuje
    rezultat. Prosti brojevi su oni koji su deljivi samo sa jedan i samim sobom, tj. 23 je prost broj
    zato sto je deljiv samo sa 1 i 23. 49 nije prost broj posto je osim sa 1 i 49 deljiv i sa 7.

    b) Dovrsiti klasu Memorija koja sluzi da alocira i dealocira regione memorije specificirane duzine.
    Memorijom se upravlja u jedinicama koje u velicini odgovaraju 1 int broju. Metod alokacije je
    first-fit. Klasa mora biti thread-safe i blokirati ako memorije nema. Kada se memorija dealocira
    samo markirati odsecak kao slobodan, bez komprimovanja. Na pocetku evidencije, sva memorije je u
    jednom velikom, slobodnom odsecku. Koliko memorije ima se specificira prilikom stvaranja promenljive
    tipa 'Memorija'

    c) Napisati funkciju koja se izvrsava kao detach-ovana, daemon-nit koja, kada se to od nje
    zatrazi, preko metode u klasi Memorija, vrsi kompakciju memorije, tj. susedne odsecke memorije
    izmedju kojih nema alociranog prostora pretvara u jedan veliki odsecak. Obezbediti ovoj niti
    da se moze na zahtev korisnika ugasiti. Komprimovanje ovde ne radi realokaciju nicega, sve sto radi
    jeste detektuje situacije gde imamo susedne odsecke koji su slobodni i spaja ih. Napisati tajmer
    koji poziva kompakciju memorije svakih deset sekundi.


    Detalje zadataka pod a, b, i c mozete videti u daljem tekstu koda gde su uvek oznaceni komentarima
    oblika
        //TODO X Tekst
    Gde je X ili A ili B ili C (u zavisnosti toga kom zadatku pripada), a tekst su instrukcije.
*/

// I/O
#include <iostream>
#include <iomanip>

// STL
#include <list>

// THREADING
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace chrono;

struct Odsecak
{
    int *m;    // pokazivac na memoriju odsceka
    int n;     // velicina koja je zauzeta
    bool free; // da li je odsecak slobodan ili ne
};

class Dijagnostika
{
private:
    mutex m;

public:
    Dijagnostika() {}
    // Stampa rezultate racunanja prostih brojeva na lep nacin
    void stampajRezultate(const char *tag, int n, int *mem)
    {
        unique_lock<mutex> l(m);
        cout << "[" << tag << "] Rezultati: " << endl;
        for (int i = 1; i <= n; i++)
        {
            cout << setfill(' ') << setw(8) << mem[i - 1] << " ";
            if (i % 9 == 0)
                cout << endl;
        }
        cout << endl;
    }

    // stampa memoriju na pregledan nacin gde se sukcesivni odsceci oznacavaju sa a, b, c, d itd.
    // Mala slova oznacavaju zauzete odsecke
    // Velika slova oznacavaju slobodne odsecke
    void stampajMapuMemorije(list<Odsecak>::iterator begin, list<Odsecak>::iterator end)
    {
        unique_lock<mutex> l(m);
        int j = 1;
        char c = 'a';
        const char offset = 'A' - 'a';
        for (auto it = begin; it != end; it++)
        {
            for (int i = 0; i < it->n; i++)
            {
                if (it->free)
                    cout << (char)(c + offset);
                else
                    cout << c;
                if (j % 8 == 0)
                    cout << " ";
                if (j >= 72)
                {
                    cout << endl;
                    j = 0;
                }
                j++;
            }
            c++;
        }
        cout << endl;
    }

    // Opsta poruka za debug
    void stampajPoruku(const char *tag, const char *msg)
    {
        unique_lock<mutex> l(m);
        cout << "[" << tag << "] " << msg << endl;
    }
};

class Memorija
{
private:
    Dijagnostika &d;
    list<Odsecak> evidencija;
    int max_size;
    mutex m;
    condition_variable cv, cv_kompakt;
    bool signal, timer;
    // First fit algoritam, dat kao ozbiljna olaksica. Ova verzija NIJE thread-safe
    // Namenjena je samo da olaksa implementaciju zauzimanja, ne moze da se koristi van
    // klase.
    list<Odsecak>::iterator firstFit(int n)
    {
        for (auto it = evidencija.begin(); it != evidencija.end(); it++)
        {
            if (it->free && it->n == n)
            {
                it->free = false;
                return it;
            }
            else if (it->free && it->n > n)
            {
                int n2 = it->n;
                it->n = n;
                it->free = false;
                Odsecak o = {m : it->m + n, n : n2 - n, free : true};
                it = evidencija.insert(++it, o);
                it--;
                return it;
            }
        }
        return evidencija.end();
    }

public:
    Memorija(int kapacitet, Dijagnostika &dd) : d(dd)
    {
        // TODO B Dopuniti po potrebi
        Odsecak pocetak;
        pocetak.free = true;
        pocetak.m = new int[kapacitet];
        pocetak.n = kapacitet;
        evidencija.push_back(pocetak);
        signal = false;
        timer = false;
        d.stampajMapuMemorije(evidencija.begin(), evidencija.end());
    }
    ~Memorija()
    {
        // TODO B Dopuniti po potrebi
    }
    Dijagnostika &getDijagnostika() const
    {
        return d;
    }

    void zavrsi()
    {
        timer = false;
        cv_kompakt.notify_one();
        // TODO C Omoguciti da se ovim terminira nit za kompakciju
    }

    void initiateCompaction()
    {
        signal = true;
        cv_kompakt.notify_one();
        // TODO C Omoguciti da se ovim zatrazi akcija od niti za kompakciju
    }

    void stampajMapuMemorije()
    {
        d.stampajMapuMemorije(evidencija.begin(), evidencija.end());
    }

    // Alocira n jedinica za int na thread-safe nacin
    int *alociraj(int n, const char *tag = "Unknown")
    {
        // TODO B Napisati thread-safe, sinhronizovan kod koji eliminse sansu stetnih preplitanja
        // i omogucava da se izvrsavanje blokira (tj. predje u WAIT stanje) ako nema dovoljno
        // memorije slobodne. N je koliko se memorije trazi. tag nam kaze koja nit trazi memoriju
        // Sluzi da bi se mogle stampati lepse poruke za dijagnostiku.
        unique_lock<mutex> l(m);
        auto it = evidencija.begin();
        while ((it = firstFit(n)) == evidencija.end())
        {
            d.stampajPoruku(tag, "Nit ceka");
            cv.wait(l);
        }
        d.stampajPoruku(tag, "Nit alocira memoriju");
        d.stampajMapuMemorije(evidencija.begin(), evidencija.end());
        return it->m;
    }
    // Oslobadja n jedinica za int na thread-safe nacin
    void dealociraj(int *p, const char *tag = "Unknown")
    {
        // TODO B Napisati thread-safe, sinhronizovan kod koji eliminse sansu stetnih preplitanja
        // Koji oslobadja memoriju na koju pokazuje p. Tag nam kaze koja nit trazi memoriju
        // Sluzi da bi se mogle stampati lepse poruke za dijagnostiku.
        unique_lock<mutex> l(m);
        for (auto it = evidencija.begin(); it != evidencija.end(); ++it)
        {
            if (it->m == p)
            {
                it->free = true;
                cv.notify_all();
                break;
            }
        }
        d.stampajPoruku(tag, "Nit oslobadja memoriju");
        d.stampajMapuMemorije(evidencija.begin(), evidencija.end());
    }
    void kompaktiraj()
    {
        // TODO C Napisati thread-safe sinhronizovan kod koji se izvrsava u beskonacnoj petlji koja
        // ceka da se otkoci zbog gasenja ili trazenja kompakcije.
        while (true)
        {
            unique_lock<mutex> l(m);
            while (signal == false && timer == false)
            {
                cv_kompakt.wait(l);
            }
            if (timer)
                return;
            cout << "Radim" << endl;
            signal = false;
            for (auto it = evidencija.begin(); it != evidencija.end(); it++)
            {
                if (it->free)
                {
                    auto ita = it;
                    for (; ita != evidencija.end(); ++ita)
                    {
                        if (ita->free)
                        {
                            it->n += ita->n;
                        }
                        else
                        {
                            break;
                        }
                    }
                    it = evidencija.erase(it, ++ita);
                }
            }
            d.stampajMapuMemorije(evidencija.begin(), evidencija.end());
        }
    }
};

// Racuna proste brojeve pocevsi od vrednosti 'od', pa sve dok ne nadje n prostih brojeva.
// rezultat smesta tamo gde pokazuje 'gde.'
void racunaj(int od, int n, int *gde)
{
    // TODO A Napisati algoritam za trazenje prostih brojeva od neke vrednosti od pa dok ne nadje n
    // prostih brojeva i smestiti rezultate u niz na koji pokazuje 'gde.'
    // Efikasnost algoritma se ne ocenjuje.
    int br_nadjenih = 0;
    int delioci = 0;
    if (od == 0)
        od++;
    while (br_nadjenih != n)
    {
        for (int i = 2; i <= od / 2; ++i)
        {
            if (od % i == 0)
            {
                delioci++;
            }
        }
        if (delioci == 0)
        {
            bool nema = true;
            for (int k = 0; k < br_nadjenih; ++k)
            {
                if (gde[k] == od)
                    nema = false;
            }
            if (nema)
                gde[br_nadjenih++] = od;
        }
        delioci = 0;
        od++;
    }
}

// Telo deattach-ovane niti za kompakciju u okviru zadatka C
void compactionDeamon(Memorija &mem)
{
    mem.kompaktiraj();
}

// Telo niti za racunanje koja se koristi za testiranje u zadacima B i C.
// n je broj blokova prostih brojeva koji se racunaju svaki od kojih je od 256 do 512 jedinica velik.
void calculationThread(const char *threadName, Memorija &mem, int n)
{
    for (int i = 0; i < n; i++)
    {
        int s = rand() % 256 + 256;
        int *x = mem.alociraj(s, threadName);
        racunaj(rand() % 10000, s, x);
        mem.getDijagnostika().stampajRezultate(threadName, s, x);
        mem.dealociraj(x, threadName);
    }
}

void timerThread(Memorija &mem, bool &active)
{
    // TODO C Napisati telo timerThread funkcije koja predstavlja telo tajmer niti tako da
    // se svakih deset sekundi inicira kompakcija memorije i tako da se timerThread nit moze
    // ugasiti spolja kroz manipulaciju 'active' promenljivom.
    while (active)
    {
        this_thread::sleep_for(seconds(10));
        mem.initiateCompaction();
    }
}

void testirajA(Dijagnostika &d)
{
    int *mem = new int[1024];
    int *p[8];
    // TODO A napraviti niz od osam niti
    thread niz[8];
    for (int i = 0; i < 8; i++)
    {
        p[i] = mem + (i * 128);
        // TODO A Pokrenuti nit tako da izvrsava funkciju racunaj tako da
        // racuna 128 prostih brojve pocevsi od 0 za prvu nit, 2000 za drugu, 4000 za trecu i tako dalje
        // i tako da smesti rezultate u memoriju na koju pokazuje p[i].
        // POMOC: parametar p[i] se mora proslediti koristeci ref()
        niz[i] = thread(racunaj, i * 2000, 128, ref(p[i]));
    }
    // TODO A join-ovati sve niti koje su pokrenute u ovom testu.
    for (int i = 0; i < 8; i++)
    {
        niz[i].join();
    }
    d.stampajRezultate("TEST1", 1024, mem);
    delete[] mem;
}

void testirajB(Memorija &mem)
{
    // Upozorenje: Test B (namerno) izaziva mrtvu petlju.
    // TODO B Napraviti niz 10 niti.
    char tags[10][10];
    thread niz[10];
    for (int i = 0; i < 10; i++)
    {
        sprintf(tags[i], "BTEST_%d", i);
        int n = rand() % 6 + 1;
        // TODO B Pokrenuti nit koja ima ime smesteno u tags[i] i koja radi sa memorijom mem i racuna
        // n blokova prostih brojeva kroz calculateThread funkciju
        niz[i] = thread(calculationThread, tags[i], ref(mem), n);
    }
    for (int i = 0; i < 10; i++)
    {
        niz[i].join();
    }
    // TODO B join-ovati sve pokrenute niti.
}

void testirajC(Memorija &mem)
{
    // TODO C Napraviti niz od 10 niti.
    char tags[10][10];
    thread niz[10];
    for (int i = 0; i < 10; i++)
    {
        sprintf(tags[i], "CTEST_%d", i);
        int n = rand() % 6 + 1;
        niz[i] = thread(calculationThread, tags[i], ref(mem), n);
        // TODO C Pokrenuti nit koja ima ime smesteno u tags[i] i koja radi sa memorijom mem i racuna
        // n blokova prostih brojeva kroz calculateThread funkciju
    }
    bool runTimer = true;
    // TODO C pokrenuti kao detach-ovane niti koje pokrecu compactionDeamon i timerThread funkcije
    // koristiti runTimer da se kontrolise da li se timer izvrsava ili ne.
    thread niti[2];
    niti[0] = thread(compactionDeamon, ref(mem));
    niti[1] = thread(timerThread, ref(mem), ref(runTimer));
    niti[0].detach();
    niti[1].detach();
    // TODO C join-ovati sve niti iz niza
    mem.zavrsi();
    runTimer = false;
}

void testirajSve()
{
    Dijagnostika d;
    Memorija mem(1024, d);
    // testirajA(d);
    //  testirajB(mem);
    testirajC(mem);
}

int main()
{
    testirajSve();
    return 0;
}