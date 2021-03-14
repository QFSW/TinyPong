#include "vector2.hpp"

Vector2& Vector2::operator+=(const Vector2& o)
{
    x += o.x;
    y += o.y;
    return *this;
}

Vector2& Vector2::operator*=(int o)
{
    x *= o;
    y *= o;
    return *this;
}

Vector2 operator+(Vector2 a, const Vector2& b)
{
    return a += b;
}

Vector2 operator*(Vector2 a, int b)
{
    return a *= b;
}
