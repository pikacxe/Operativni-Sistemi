/*
Napraviti program koji prihvata proizvoljan broj linija
sa standardnog ulaza i zatim ih ispisuje u redosledu kojim su uneti.
*/

#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<string> v; //STL vektor koja sadrzi linije unesene sa tastature
    string s;

    while(getline(cin,s)){
        v.push_back(s);
    }

    vector<string>:: iterator it;
    for(it = v.begin(); it != v.end(); it++){

        cout<< *(it) << endl;
    }
}
