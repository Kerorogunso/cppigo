#pragma once
#include <string>
#include "cppigo_exports.h"

class CPPIGO_DLL Player
{
public:
    std::string name;
    std::string color;

    Player();
    Player(const std::string &name);
    Player(const std::string &name, const std::string &color);
    Player(const Player& player);

    void printName();
    float getKomi();
    bool operator==(const Player& otherPlayer);
};
