#include <vector>

using namespace std;

class Goban
{
    public:
        vector<int> board;
    
    void placeStone(vector<int> positon);
};

class Group
{
    public:
        vector<int> stones;
        int liberties;

    int getLiberties();
    bool isCaptured();
};