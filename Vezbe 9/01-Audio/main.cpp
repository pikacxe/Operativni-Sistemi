/*
    Napisati konkurentni program koji simulira audio podsistem racunara. Sistem se sastoji primarno
    od centralnog audio bafera koji sadrzi odredjeni broj semplova. Radi jednostavnosti, smatra se
    kao dato da je stopa semplovanja 32-bitna sa 8Khz. To znaci da 1 sekunda audio zapisa u ovom
    formatu je 8000 32-bitnih float vrednosti, odn. 32 kilobajta. Svaki sempl predstavlja 25ms
    odnosno 200 sempl vrednosti. Bafer audio uredjaja je dugacak 2 sekunde, odn. 16000 sempl vrednosti, 
    odn. 80 semplova, ali ovo treba da je podesivo. 
    
    Sempl sadrzi ne samo podatke, nego i trenutak u vremenu kada je namenjen da se 
    pusti. Radi jednostavnosti, predpostavimo da su ti trenuci uvek specificirani u jedinicama od 25ms
    pocevsi od nekog zamisljenog nultog momenta. 

    a) Dovrsiti funkciju koja generise sinusni ton date frekvencije, sa datom stopom semplovanja,
    za dat vremenski trenutak i smesta je u prosledjen bafer. Zatim pokrenuti vise niti koje paralelno
    u nekoliko bafera generisu tonove frekvencija 440Hz, 554.356Hz, 660Hz (pravimo akord). 

    b) Modifikovati i dovrsiti klasu Bafer tako da vodi racuna o:
        i) Popunjenosti, tako da ako se iz nje cita a prazna je ili se u nju pise a puna je, se 
        blokira. 
        ii) Redosledu, tako da se ubaceni semplovi uvek sortiraju po svom vremenu. 
        iii) Sumiranju: Semplovi namenjeni za isto vreme se sabiraju, vodeci racuna da je 
        1.0 maksimalna vrednost.
        iv) Prioritetu: Semplovi koji 'kasne' odnosno nalaze se u proslosti u odnosu na najstarije
        se odbacuju. 
    
    c) Napisati pozadinsku nit koja se aktivira na tajmer (koji okida svakih 25ms) i cita u vremenu
    najskoriji sempl koji se 'pusta' (ispisuje na ekran). Pozadinska nit, i pozadinska nit tajmera se
    moraju zavrsiti na zahtev glavne niti. 


    Detalje zadataka pod a, b, i c mozete videti u daljem tekstu koda gde su uvek oznaceni komentarima
    oblika 
        //TODO X Tekst 
    Gde je X ili A ili B ili C (u zavisnosti toga kom zadatku pripada), a tekst su instrukcije. 
    BONUS ZADATAK 1: Tako napraviti kod da se *sva* neophodna memorija za cuvanje uzoraka u audio baferu
    alocira na samom pocetku izvrsavanja, i da nema dalje dealokacije i alokacije tokom izvrsavanja.
    BONUS DOMACI ZADATAK 2: Generisati audio fajl tona koji ovaj program generise uz opis toga kako ste ga
    napravili. 
*/
//I/O
#include <iostream>
#include <iomanip>

//STL
#include <list>

//Multithreading
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable> 

//Other
#include <cmath>
#include <cstring>

using namespace std;
using namespace chrono;

struct Sample{
    float data[200];
    int time;
};

class Dijagnostika{
    private:
        mutex m;
    public:
        Dijagnostika() {}
        /*
            stampajSample uzme jedan uzorak audio signala i prikaze ga na ekranu "graficki"
            Uzme srednju vrednost delica sample-a (njih 20) i sracuna kolika je velicina proseka
            u skali od 0 do 15. 

            Onda ga prikaze na 5 linija teksta u zavisnosti od jacine signala na sledeci nacin
            13-15|    | Prazno znaci da signal nije stigao dovde u jacini
            10-12| -  | Znak - znaci da je u ovoj kategoriji signal na poziciji 2, tj. 11
            07-09| #  | Znak # znaci da je signal stigao da i potpuno popunio ovaj nivo jacine
            04-06| #  | Znak # znaci da je signal stigao da i potpuno popunio ovaj nivo jacine
            01-03| #  | Znak # znaci da je signal stigao da i potpuno popunio ovaj nivo jacine
        */
        void stampajSample(Sample *s){
            unique_lock<mutex> l(m);
            cout << "Playing sample at " << s->time << endl;
            int p[20] = {0};
            for(int i = 0; i < 20; i++){
                float avg = 0;
                for(int j = 0; j < 10;j++){
                    avg += s->data[i * 10 + j];
                }
                avg = avg / 10.0f;
                int x = round((avg * 15.0f));
                p[i] = x;
            }
            for(int i = 4; i >= 0; i--){
                for(int j = 0; j < 20;j++){
                    if(p[j] > i * 3){
                        if(p[j] > i * 3 + 2){
                            cout << "#";
                        }else if(p[j] > i * 3 + 1){
                            cout << "-";
                        }else{
                            cout << "_";
                        }
                    }else{
                        cout << " ";
                    }
                }
                cout << "\n";
            }

            cout << endl;
        }

        //Stampanje uzorka za dijagnostiku u sirovoj formi, tj. kao brojevi.
        //Nema sinhronizaciju zato sto je tu samo za debugovanje
        void stampajSampleRaw(Sample *s){
            for(int i = 0; i < 200; i++){
                cout << s->data[i] << endl;
            }
        }

        //sluzi za debagovanje
        void stampajPoruku(const char* tag, const char* msg){
            unique_lock<mutex> l(m);
            cout << "[" << tag << "] " << msg << endl;
        }
};

class Bafer {
    private:
        Dijagnostika& d;
        //TODO B,C Dodati po potrebi prosirenja
    public:
        Bafer(Dijagnostika& dd, int cap) : d(dd){         //TODO B,C Dodati po potrebi prosirenja

        }
        ~Bafer(){         //TODO B,C Dodati po potrebi prosirenja

        }
        Dijagnostika& getDijagnostika(){
            return d;
        }
        void store(Sample& in){
            //TODO B Napraviti thread-safe cuvanje uzorka ovde u skladu sa zadatkom
        }
        void load(Sample& out){
            //TODO B Napraviti thread-safe ucitavanje uzorka ovde u sladu sa zadatkom
        }

        void play(){
            //TODO C napraviti beskonacnu petlju ovde koja se izvrsava pozadniski i inicira iz tajmer
            //niti, a 'pusta' zvuk, tj. ispisuje tekuci sample na ekran.  
        }

        void initiatePlay(){
            //TODO C mehanizam koji se poziva iz tajmer niti i 'otkoci' nit play 
        }
        void dumpBuffer(){
            //TODO B Ispisuje ceo bafer na ekran 
        }

        void dumpBufferRaw(){
            //TODO OPCIONO Ispisuje ceo bafer na ekran u sirovoj formi
        }
};

//Funkcija koja radi odgovarajucu matematiku koja racuna uzorak sinusnog tona datih parametara
//frequency - frekvencija koja se generise, u Hz
//time - Koliko se vec emituje zvuk (neophodno za racunanje faze tona) u sekundama
//length - Duzina uzorka koji se generise, u sekundama
//sampleRate - stopa uzorkovanja, u Hz
//volume - Jacina zvuka od 0 do 1.0
//data - gde da se smeste podaci
void computeSample(double frequency, double time, double length, double sampleRate, double volume, float* data){
    //sracunati fazu
    double phase = (time * frequency);
    phase = phase - floor(phase);
    phase = phase * 2 * M_PI; 
    int n = ceil(length * sampleRate); 
    double incr = (2 * M_PI * frequency) / sampleRate;
    double t = phase;
    for(int i = 0; i < n; i++){
        *(data + i) = (float)(volume * ((sin(t) / 2) + 0.5));//ocuvati raspon od 0 do 1. 
        t+= incr;
    }
}

void testirajA(Dijagnostika& d){
    //TODO A Izracunati tri sample-a duzine od 0.025s frekvencija 440, 554.356, 660
    //sa stopom uzorkovanja od 8kHz i jacinom zvuka od 100% u paraleli u tri nezavisne niti. 
    //Zatim ispisati sva tri uzorka. 
}


void toneGenerator(double freq, double length, double volume, Bafer& b){
    //TODO B Funkcija koja cini telo niti za proizvodnju tona koja sluzi da generise uzorke 
    //prave duzine (25ms) i smesta ih sa odgovarajucim parametrom vremena u Bafer koristeci 
    //metodu store
    //POMOC: Da bi za datu duzinu tona saznali broj uzoraka moze vam pomoci sledeci kod:
    //int n = ceil(length / 0.025);
}

void testirajB(Bafer& b){
    //TODO B Pokrenuti tri niti koje izvrsavaju toneGenerator sa parametrima 440Hz, 554.356Hz
    //660Hz, duzinama od po 1s, i jacinom zvuka od 30%. 
    //kada niti zavrse, ispisati ceo bafer. 
}


//TODO C Napraviti ovde telo niti koje sluzi da automatski tempirano, 'pusta' (ispisuje na ekran)
//tekuci uzorak iz bafera svakih 25ms kroz initiatePlay. 

//Telo niti za pustanje zvuka
void daemonThread(Bafer& b){
    b.play();
}

void testirajC(Bafer& b){
    //TODO C pokrenuti 5 niti
    //Tri su generatori tona sa parametrima: 440.0Hz, 554.356Hz, 660.0Hz frekvencije
    //5.0s duzina
    //30% jacina zvuka
    //jedna je tajmerska nit
    //i konacna je nit za pustanje zvuka tj. ispis na ekran cije je telo 'daemonThread'
    //Nit za pustanje zvuka i nit za tajmer su pozadinske
    //TODO C Pustiti korisnika da prekine program na zahtev tako sto otkuca 'q'
    //TODO C Pred kraj izvrsavanja, zaustaviti pozadinske niti tako sto im se izda komanda. 
}

int main(){
    //TODO A Pokrenuti 'testirajA'
    //TODO B Pokrenuti 'testirajB'
    //TODO C Pokrenuti 'testirajC'
    return 0;
}