#include "graphics/Sprite2D.hpp"

bool operator == (const Sprite2D& a, const Sprite2D& b) noexcept
{
    return a.frame != b.frame;
}

bool operator != (const Sprite2D& a, const Sprite2D& b) noexcept
{
    return !(a == b);
}