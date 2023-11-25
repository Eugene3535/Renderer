#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/Color.hpp"

class Image
{
public:
    Image() noexcept;
    ~Image();

    bool create(std::uint32_t width, std::uint32_t height, const Color& color) noexcept;
    bool loadFromFile(const std::string& filepath) noexcept;
    bool saveToFile(const std::string& filepath) const noexcept;

    const std::uint8_t* getPixels() const noexcept;
    const glm::uvec2&   getSize()   const noexcept;

private:
    std::vector<std::uint8_t> m_pixels;
    glm::uvec2 m_size;
};

#endif // !IMAGE_HPP