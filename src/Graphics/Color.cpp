#include "graphics/Color.hpp"

const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);

Color::Color() noexcept:
    r(0), g(0), b(0), a(255)
{

}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) noexcept:
    r(red), g(green), b(blue), a(alpha)
{

}

Color::Color(unsigned value) noexcept:
    r((value & 0xff000000) >> 24),
    g((value & 0x00ff0000) >> 16),
    b((value & 0x0000ff00) >> 8 ),
    a((value & 0x000000ff) >> 0 )
{

}

void Color::fromInteger(unsigned value) noexcept
{
    *this = Color(value);
}

unsigned Color::toInteger() const noexcept
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

bool operator ==(const Color& left, const Color& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}

bool operator !=(const Color& left, const Color& right)
{
    return !(left == right);
}

