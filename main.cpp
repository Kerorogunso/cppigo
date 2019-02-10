#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
    GoGame go;
    while (true)
    {
        string move;
        cout << "What's your move:" << endl;
        cin >> move;
        tuple<int, int> parsedMove = parseMove(move);
        go.play(parsedMove);
    }
}