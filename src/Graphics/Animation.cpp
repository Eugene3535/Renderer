#include "graphics/Sprite2D.hpp"
#include "graphics/Animation.hpp"

bool operator == (const Animation& a, const Animation& b) noexcept
{
    return a.sprites == b.sprites;
}

bool operator != (const Animation& a, const Animation& b) noexcept
{
    return !(a == b);
}