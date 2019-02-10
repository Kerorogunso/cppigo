#pragma once

#include "utilities_exports.h"

struct ColourRGBf
{
    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;
};

bool UTILITIES_DLL operator==(const ColourRGBf &lhs, const ColourRGBf &rhs);

bool UTILITIES_DLL operator!=(const ColourRGBf &lhs, const ColourRGBf &rhs);

constexpr ColourRGBf kBlack{ 0.f, 0.f, 0.f };
constexpr ColourRGBf kWhite{ 1.f, 1.f, 1.f };
constexpr ColourRGBf kRed{ 1.f, 0.f, 0.f };
constexpr ColourRGBf kGreen{ 0.f, 1.f, 0.f };
constexpr ColourRGBf kBlue{ 0.f, 0.f, 1.f };