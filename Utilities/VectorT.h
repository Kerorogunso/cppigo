#pragma once

template <typename T>
struct Vector3
{
    T x = {};
    T y = {};
    T z = {};
};

template <typename T>
struct Vector2
{
    T x = {};
    T y = {};
};

using Vector3f = Vector3<float>;

struct ColourRGBf
{
    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;