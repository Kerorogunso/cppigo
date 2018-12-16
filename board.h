#pragma once
#include <vector>

using namespace std;

class Goban
{
    public:
        int boardSize;
        vector<int> board;
    
    void placeStone(vector<int> positon);
    int getBoardSize();
};

class Group
{
    public:
        vector<int> stones;
        int liberties;

    int getLiberties();
    bool isCaptured();
};