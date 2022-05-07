#ifndef ANTONIM_H
#define ANTONIM_H

#include <map>
#include <string>

using namespace std;

// Funkcija koja će biti iskorišćena kao funkcija niti - u zadatom rečniku (parametar "recnik") pronaći suprotnu reč (antonim) od zadate reči "rec"
//
// recnik   - Rečnik koji sadrži parove reč - reč suprotnog značenja
// rec      - Reč za koju se traži reč suprotnog značenja
// rezultat - Reč suprotnog značenja (ako je pronađena, ako nije pronađena ovde treba da se upiše prazan string)
//
void nadjiAntonim(map<string, string> recnik, string rec, string& rezultat)
{
    rezultat = "";
    map<string,string>::iterator it;
    for(it = recnik.begin(); it != recnik.end(); ++it){
        if(it->first == rec){
            rezultat = it->second;
        }
        else if(it -> second == rec){
            rezultat = it->first;
        }
    }

    // Implementirati ...
}

#endif // ANTONIM_H
