#include <string>

using namespace std;

class Player 
{
    public:
    string name;

    Player();
    Player(const string name);

    void printName();
};

class BlackPlayer : public Player 
{
    const float komi = 0.0;
};

class WhitePlayer : public Player
{
    const float komi = 7.5;
};