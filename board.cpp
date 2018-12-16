#include <vector>

#include "board.h"

using namespace std;

void Goban::placeStone(vector<int> position)
{
    vector<int> a;
}

int Goban::getBoardSize()
{
    return 13;
}

int Group::getLiberties()
{
    return liberties;
}

bool Group::isCaptured()
{
    if (liberties == 0)
    {
        return true;
    }
    return false;
}