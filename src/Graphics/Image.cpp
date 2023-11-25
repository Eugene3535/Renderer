#include <iostream>
#include <algorithm>
#include <cctype>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "graphics/Image.hpp"

Image::Image() noexcept:
    m_size(0u, 0u)
{
}

Image::~Image()
{
}

bool Image::create(std::uint32_t width, std::uint32_t height, const Color& color) noexcept
{
    if (width && height)
    {
        std::vector<std::uint8_t> newPixels(width * height * 4);
    
        std::uint8_t* ptr = &newPixels[0];
        std::uint8_t* end = ptr + newPixels.size();

        while (ptr < end)
        {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }
    
        m_pixels.swap(newPixels);
        
        m_size.x = width;
        m_size.y = height;

        return true;
    }

//  Dump the pixel buffer
    std::vector<std::uint8_t>().swap(m_pixels);
    m_size.x = 0;
    m_size.y = 0;

    return false;
}

bool Image::loadFromFile(const std::string& filepath) noexcept
{
    m_pixels.clear();

    std::int32_t width = 0;
    std::int32_t height = 0;
    std::int32_t bytePerPixel = 0;
    glm::uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &bytePerPixel, STBI_rgb_alpha);

    if (!data)
        return false;

    m_size = { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };

    m_pixels.resize(static_cast<size_t>(width * height * 4));
    std::memcpy(&m_pixels[0], data, m_pixels.size());
    stbi_image_free(data);

    return true;
}

bool Image::saveToFile(const std::string& filename) const noexcept
{
    if (!m_pixels.empty() && (m_size.x > 0) && (m_size.y > 0))
    {
        // Extract the extension
        const std::size_t dot = filename.find_last_of('.');
        std::string extension = (dot != std::string::npos) ? filename.substr(dot + 1) : std::string();

        if(extension.empty())
            return false;

        std::transform(extension.cbegin(), extension.cend(), extension.begin(),
                [](unsigned char c) { return std::tolower(c); });

        int width  = static_cast<std::int32_t>(m_size.x);
        int height = static_cast<std::int32_t>(m_size.y);

        if (extension == "bmp") // BMP
        {
            if (stbi_write_bmp(filename.c_str(), width, height, 4, m_pixels.data()))
                return true;
        }
        else if (extension == "tga") // TGA
        {     
            if (stbi_write_tga(filename.c_str(), width, height, 4, m_pixels.data()))
                return true;
        }
        else if (extension == "png") // PNG
        {        
            if (stbi_write_png(filename.c_str(), width, height, 4, m_pixels.data(), 0))
                return true;
        }
        else if (extension == "jpg" || extension == "jpeg") // JPG
        {     
            if (stbi_write_jpg(filename.c_str(), width, height, 4, m_pixels.data(), 90))
                return true;
        }
    }

    std::cerr << "Failed to save image \"" << filename << "\"" << std::endl;

    return false;
}

const std::uint8_t* Image::getPixels() const noexcept
{
    return m_pixels.data();
}

const glm::uvec2& Image::getSize() const noexcept
{
    return m_size;
}