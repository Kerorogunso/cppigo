#include <iostream>

#include "player.h"
#include "board.h"

using namespace std;

int main()
{
    Player albert;
    cout << albert.name << endl;
    albert.name = "Foo";
    albert.printName();
    system("pause");
}