#include "stdafx.h"

#include <iostream>
#include <stdexcept>

#include "player.h"

using namespace std;

Player::Player()
{
    name = "Player 1";
    color = kBlack;
};

Player::Player(const string& name)
{
    this->name = name;
    this->color = kBlack;
};

Player::Player(const Player& player)
{
    this->name = player.name;
    this->color = player.color;
}

Player::Player(const string& name, const ColourRGBf& color)
{
    this->name = name;
    this->color = color;
};

void Player::printName()
{
    cout << name << endl;
}

float Player::getKomi()
{
    if (color == kBlack)
    {
        return 0;
    }
    else if (color == kWhite)
    {
        return 7.5;
    }

    // Should not get here.
    throw std::invalid_argument("Player color not identifiable.");
}
bool Player::operator==(const Player& otherPlayer)
{
    if (this->name != otherPlayer.name || this->color != otherPlayer.color)
    {
        return false;
    }
    return true;
}
