#ifndef COLOR_HPP
#define COLOR_HPP

class Color
{
public:
    Color() noexcept;
    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) noexcept;
    explicit Color(unsigned value) noexcept;

    void fromInteger(unsigned value) noexcept;
    unsigned toInteger()       const noexcept;

    static const Color Black;       
    static const Color White;       
    static const Color Red;         
    static const Color Green;       
    static const Color Blue;        
    static const Color Yellow;      
    static const Color Magenta;     
    static const Color Cyan;        
    static const Color Transparent; 

    unsigned char r; 
    unsigned char g; 
    unsigned char b; 
    unsigned char a;
};

bool operator ==(const Color& left, const Color& right);
bool operator !=(const Color& left, const Color& right);

#endif

