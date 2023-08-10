#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

class Color
{
public:
    Color() noexcept;
    Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255) noexcept;
    explicit Color(std::uint32_t value) noexcept;

    void fromInteger(std::uint32_t value) noexcept;
    std::uint32_t toInteger()       const noexcept;

    static const Color Black;       
    static const Color White;       
    static const Color Red;         
    static const Color Green;       
    static const Color Blue;        
    static const Color Yellow;      
    static const Color Magenta;     
    static const Color Cyan;        
    static const Color Transparent; 

    std::uint8_t r; 
    std::uint8_t g; 
    std::uint8_t b; 
    std::uint8_t a;
};

bool operator ==(const Color& left, const Color& right);
bool operator !=(const Color& left, const Color& right);

#endif

