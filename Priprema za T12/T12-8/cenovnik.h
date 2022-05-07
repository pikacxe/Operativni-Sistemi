#ifndef CENOVNIK_H
#define CENOVNIK_H

#include <string>
#include <map>

using namespace std;

void nadjiPovoljno(map<string, double> a, map<string, double> b, map<string, double> &jeftino)
{
    map<string, double>::iterator it;
    for (it = a.begin(); it != a.end(); ++it)
    {
        map<string, double>::iterator pom = b.find(it->first);
        if(it->second > pom->second){
            jeftino.emplace(it->first,pom->second);
        }
        else{
            jeftino.emplace(it->first,it->second); 
        }
    }
    // Implementirati ...
}

#endif // CENOVNIK_H
