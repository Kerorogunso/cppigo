#pragma once
#include <string>

using namespace std;

class Player 
{
    public:
        string name;
        string color;

        Player();
        Player(const string name);
        Player(const string name, const string color);
        Player(const Player& player);

        void printName();
        float getKomi();
};
