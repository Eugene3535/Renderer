#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

#include "Graphics/2D/Texture2D.hpp"

bool LoadImageFromFile(const std::string& filepath, std::vector<glm::uint8_t>& pixels, glm::ivec2& size)
{
    pixels.clear();

    std::int32_t bytePerPixel = 0;
    glm::uint8_t* pData = stbi_load(filepath.c_str(), &size.x, &size.y, &bytePerPixel, STBI_rgb_alpha);

    if (!pData)
        return false;

    pixels.resize(static_cast<size_t>(size.x * size.y * 4));
    std::memcpy(&pixels[0], pData, pixels.size());
    stbi_image_free(pData);

    return true;
}

void CreateTextureFromImage(const std::vector<glm::uint8_t>& pixels, Texture2D& texture)
{
	glm::uint32_t* pTexture = &texture.texture;

	glGenTextures(1, pTexture);

	glBindTexture(GL_TEXTURE_2D, *pTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.size.x, texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SetTexture2DRepeated(glm::uint32_t texture, bool repeat)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    if (repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        const static float border_color[] { 0.0f, 0.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void SetTexture2DSmooth(glm::uint32_t texture, bool smooth)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    if (smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
