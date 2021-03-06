#ifndef RECNIK_H
#define RECNIK_H

#include <map>
#include <string>

using namespace std;

// Funkcija koja će biti iskorišćena kao funkcija niti - od zadatog englesko-srpskog rečnika ona pravi obrnuti rečnik - srpsko-engleski
//
// engSrp - Englesko-srpski rečnik (ključ je reč na engleskom, podatak je njen prevod na srpski)
// srpEng - Srpsko-engleski rečnik (ključ je reč na srpskom, podatak je njen prevod na engleski) - ovaj parametar treba da se popuni izvršenjem ove funkcije
//
void napraviRecnik(map<string, string> engSrp, map<string, string>& srpEng)
{
    map<string, string>::const_iterator it;
    for(it = engSrp.begin(); it != engSrp.end(); ++it){
        // std::pair<string,string> novi = pair<string,string>(it->second,it->first);
        // srpEng.insert(it,novi);
        srpEng.emplace(it->second, it->first);
    }
}


#endif // RECNIK_H
