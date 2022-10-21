#include <iostream>

using namespace std;

int value = 4096*5;

void konvertuj()
{

    int i = 0;
    while (value > 1024)
    {
        value = value >> 10;
        i++;
    }
    switch (i)
    {
    case 0:
        cout << "Vrednost: " << value << 'B' << endl;
        break;
    case 1:
        cout << "Vrednost: " << value << 'K' << endl;
        break;
    case 2:
        cout << "Vrednost: " << value << 'M' << endl;
        break;
    default:
        break;
    }
}

int main()
{

    cout << (value >> 12) << endl;
    return 0;
}