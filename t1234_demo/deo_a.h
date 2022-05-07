#ifndef _1234_DEO_A_H
#define _1234_DEO_A_H

#include <random>
#include <thread>
#include <vector>

using namespace std;

typedef vector<float>::const_iterator ci;
typedef vector<float>::const_reverse_iterator cri;

/**
 * Provera da li je redosled elemenata u segmentu prvog vektora brojeva obrnut redosled segmenta drugog vektora brojeva.
 *
 * @param a_pocetak pocetak segmenta prvog vektora brojeva
 * @param a_kraj kraj segmenta prvog vektora brojeva
 * @param b_pocetak pocetak segmenta drugog vektora brojeva
 * @return da li je drugi vektor brojeva obrnutog redosleda u odnosu na prvi
 */
void jesu_li_isti_elementi(ci a_pocetak, ci a_kraj, cri b_pocetak, bool &rez)
{
    // Implementiraj A deo 1
    for (auto it = a_pocetak; it != a_kraj; ++it, b_pocetak++)
    {
        if ((*it) != (*b_pocetak))
        {
            rez = false;
            return;
        }
    }
    rez = true;
}

/**
 * Paralelna/konkurentna provera da li je redosled brojeva u drugom vektoru brojeva obrnut u odnosu na prvi.
 * Broj niti zavisi od slucaja jer se za svaku nit nasumicno odredjuje velicina segmenta vektora za koji se vrsi provera.
 * Velicina segmenta koju obradjuje jedna nit moze biti bilo sta izmedju jednog i pet elemenata
 *
 * @param a prvi vektor brojeva
 * @param b drugi vektor brojeva
 * @return da li je drugi vektor brojeva obrnutog redosleda u odnosu na prvi
 */
bool proveri_obrnutost(vector<float> a, vector<float> b)
{
    // Implementiraj A deo 2
    vector<int> delovi;
    int n = a.size();
    int sum_delovi = 0;
    while (sum_delovi < n)
    {
        int temp = rand() % 5 + 1;
        delovi.push_back(temp);
        sum_delovi += temp;
    }
    int n1 = delovi.size();
    bool rez[n1];
    thread niz[n1];
    sum_delovi = 0;
    ci ita = a.cbegin();
    cri itb = b.crbegin();
    for (int i = 0; i < n1; ++i)
    {
        sum_delovi += delovi[i];
        niz[i] = thread(jesu_li_isti_elementi, ita, (sum_delovi < n) ? ita + delovi[i] : a.cend(), itb, ref(rez[i]));
        ita += delovi[i];
        itb += delovi[i];
    }
    for (int i = 0; i < n1; ++i)
    {
        niz[i].join();
    }
    bool rezultat = true;
    for (int i = 0; i < 4; ++i)
    {
        rezultat = rezultat && rez[i];
    }
    return rezultat;
}

#endif //_1234_DEO_A_H
