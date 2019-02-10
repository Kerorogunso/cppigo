#include "stdafx.h"

#include "Colours.h"

bool operator==(const ColourRGBf &lhs, const ColourRGBf &rhs)
{
    return lhs.red == rhs.red && //
        lhs.green == rhs.green && //
        lhs.blue == rhs.blue;
}

bool operator!=(const ColourRGBf &lhs, const ColourRGBf &rhs)
{
    return !(lhs == rhs);
}