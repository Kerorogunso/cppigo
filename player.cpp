#include <iostream>
#include "player.h"

using namespace std;

Player::Player()
{
    name = "Player 1";
}

Player::Player(const string name)
{
    this->name = name;
}

void Player::printName()
{
    cout << this->name << endl;
}