#pragma once

#include "VectorT.h"

template <typename T>
struct Rectangle
{
    T x = {};
    T y = {};
    T width = {};
    T height = {};

    template <typename U>
    bool containsPoint(const Vector3<U> &point)
    {
        return point.x >= x && //
            point.y >= y && //
            point.x <= (x + width) && //
            point.y <= (y + height);
    }
};