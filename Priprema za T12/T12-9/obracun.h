#ifndef OBRACUN_H
#define OBRACUN_H

#include <string>
#include <map>
#include <list>

using namespace std;

void obracunajUkupno(list<string> kupljeno, map<string, double> cenovnik, double& ukupno)
{
    // Implementirati ...
    for(auto it = kupljeno.begin(); it != kupljeno.end(); ++it){
       // ukupno += cenovnik.find(*it)->second;
        ukupno += cenovnik[*it];
    }
}
#endif // OBRACUN_H
