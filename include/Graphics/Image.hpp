#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "Geometry/Vector2.hpp"

class Image
{
public:
    Image() noexcept;
    Image(const Image& other) = delete;
	Image(const Image&& other) = delete;
	Image& operator = (const Image& other) = delete;
	Image& operator = (const Image&& other) = delete;
    ~Image();

    bool loadFromFile(const std::string& filepath) noexcept;

    const Vector2i&     getSize() const noexcept;
    const std::uint8_t* getData() const noexcept;

private:
    std::vector<std::uint8_t> m_pixels;

    Vector2i m_size;
};

#endif