#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/Color.hpp"

class Image
{
public:
    Image() noexcept;
    ~Image();

    bool create(unsigned width, unsigned height, const Color& color) noexcept;
    bool loadFromFile(const std::string& filepath) noexcept;
    bool saveToFile(const std::string& filepath) const noexcept;

    const unsigned char* getPixels() const noexcept;
    const glm::uvec2&    getSize()   const noexcept;

private:
    std::vector<std::uint8_t> m_pixels;
    glm::uvec2 m_size;
};

#endif // !IMAGE_HPP