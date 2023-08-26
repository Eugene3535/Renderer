#include "Graphics/Texture2D.hpp"

#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

bool LoadImageFromFile(const std::string& filepath, std::vector<unsigned char>& pixels, int& width, int& height)
{
    pixels.clear();

    int bytePerPixel = 0;
    unsigned char* pData = stbi_load(filepath.c_str(), &width, &height, &bytePerPixel, STBI_rgb_alpha);

    if (!pData)
        return false;

    pixels.resize(static_cast<size_t>(width * height * 4));
    std::memcpy(&pixels[0], pData, pixels.size());
    stbi_image_free(pData);

    return true;
}

void CreateTextureFromImage(const std::vector<unsigned char>& pixels, Texture2D& texture)
{
	unsigned* pTexture = &texture.texture;

	glGenTextures(1, pTexture);

	glBindTexture(GL_TEXTURE_2D, *pTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SetTexture2DRepeated(unsigned texture, bool repeat)
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

void SetTexture2DSmooth(unsigned texture, bool smooth)
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
