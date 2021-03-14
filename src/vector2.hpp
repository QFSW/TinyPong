#pragma once

struct Vector2
{
    int x = 0;
    int y = 0;

    Vector2& operator+=(const Vector2& o);
    Vector2& operator*=(int o);
};

Vector2& operator+(Vector2 a, const Vector2& b);
Vector2& operator*(Vector2 a, int b);