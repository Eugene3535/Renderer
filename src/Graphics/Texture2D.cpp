#include <glad/glad.h>

#include "graphics/Texture2D.hpp"

Texture2D::Texture2D() noexcept:
    m_size(),
    m_texture(0u),
    m_isSmooth(false),
    m_isRepeated(false)
{
}

Texture2D::~Texture2D()
{
    if(m_texture)
        glDeleteTextures(1, &m_texture);
}

bool Texture2D::loadFromFile(const std::string& filepath) noexcept
{
    Image image;

    return image.loadFromFile(filepath) && loadFromImage(image);
}

bool Texture2D::loadFromImage(const Image& image) noexcept
{
    if(image.getPixels() == nullptr)
        return false;

    m_size = image.getSize();
    const int width  = static_cast<int>(m_size.x);
    const int height = static_cast<int>(m_size.y);

	glGenTextures(1, &m_texture);
    Texture2D::bind(this);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
    Texture2D::bind(nullptr);

    return true;
}

void Texture2D::setSmooth(bool smooth) noexcept
{
    if(m_isSmooth != smooth)
    {
        m_isSmooth = smooth;
        Texture2D::bind(this);

        if (m_isSmooth)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        Texture2D::bind(nullptr);
    }
}

void Texture2D::setRepeated(bool repeate) noexcept
{
    if(m_isRepeated != repeate)
    {
        m_isRepeated = repeate;
        Texture2D::bind(this);

        if (m_isRepeated)
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

        Texture2D::bind(nullptr);
    }
}

bool Texture2D::isSmooth() const noexcept
{
    return m_isSmooth;
}

bool Texture2D::isRepeated() const noexcept
{
    return m_isRepeated;
}

unsigned Texture2D::getNativeHandle() const noexcept
{
    return m_texture;
}

const glm::uvec2& Texture2D::getSize() const noexcept
{
    return m_size;
}

void Texture2D::bind(const Texture2D* texture) noexcept
{
    if(texture)
        glBindTexture(GL_TEXTURE_2D, texture->m_texture);
    else
        glBindTexture(GL_TEXTURE_2D, 0);
}