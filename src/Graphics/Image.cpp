#include "Graphics/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

#include "stb_image.h"

Image::Image() noexcept
{
}

Image::~Image()
{
}

bool Image::loadFromFile(const std::string& filepath) noexcept
{
    int bytePerPixel = 0;
    unsigned char* pData = stbi_load(filepath.c_str(), &m_size.x, &m_size.y, &bytePerPixel, STBI_rgb_alpha);

    if ( ! pData )
        return false; 

    m_pixels.resize(m_size.x * m_size.y * 4);
    memcpy(m_pixels.data(), pData, m_pixels.size()); 

    return true;
}

const glm::ivec2& Image::getSize() const noexcept
{
    return m_size;
}

const std::uint8_t* Image::getData() const noexcept
{
    return m_pixels.data();
}