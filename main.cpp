#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
    Player albert;
    cout << albert.name << endl;
    albert.name = "Foo";
    albert.printName();
    Goban goban;
    cout << goban.getBoardSize() << endl;
    GoGame gogame;
    gogame.black.printName();
    system("pause");
}