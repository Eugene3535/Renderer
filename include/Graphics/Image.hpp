#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

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

    const glm::ivec2&   getSize() const noexcept;
    const std::uint8_t* getData() const noexcept;

private:
    std::vector<std::uint8_t> m_pixels;

    glm::ivec2 m_size;
};

#endif