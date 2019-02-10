#pragma once
#include <string>
#include "Utilities/Colours.h"
#include "cppigo_exports.h"

class CPPIGO_DLL Player
{
public:
    std::string name;
    ColourRGBf color;

    Player();
    Player(const std::string &name);
    Player(const std::string &name, const ColourRGBf &color);
    Player(const Player& player);

    void printName();
    float getKomi();
    bool operator==(const Player& otherPlayer);
};
