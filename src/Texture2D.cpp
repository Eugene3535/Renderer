#include "Texture2D.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #define STBI_ONLY_PNG
// #define STBI_ONLY_JPEG

// #include "stb_image.h"

// Texture2D::Texture2D()  noexcept: 
//     m_size(),
//     m_handle(0),
//     m_isRepeated(false),
//     m_isSmooth(false)
// {
// }

// Texture2D::~Texture2D()
// {
//     if(m_handle) 
//         glDeleteTextures(1, &m_handle);
// }

// bool Texture2D::loadFromFile(const std::string& filepath) noexcept
// {
//     if (m_handle) 
//         glDeleteTextures(1, &m_handle);

//     glGenTextures(1, &m_handle);

//     setRepeated(false);
//     setSmooth(false);

//     int bytePerPixel = 0;
//     unsigned char* pData = stbi_load(filepath.c_str(), &m_size.x, &m_size.y, &bytePerPixel, STBI_rgb_alpha);

//     if ( ! pData)
//         return false;
    
//     glBindTexture(GL_TEXTURE_2D, m_handle);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
//     glGenerateMipmap(GL_TEXTURE_2D);
//     unbind();   

//     m_pixels.resize(m_size.x * m_size.y * 4);
//     memcpy(m_pixels.data(), pData, m_pixels.size());

//     stbi_image_free(pData); 

//     return true;
// }

// void Texture2D::bind(GLuint slot) noexcept
// {
//     glActiveTexture(GL_TEXTURE0 + slot);
//     glBindTexture(GL_TEXTURE_2D, m_handle);
// }

// void Texture2D::unbind() noexcept
// {
//     glBindTexture(GL_TEXTURE_2D, 0);        
// }

// void Texture2D::setRepeated(bool repeat) noexcept
// {
//     glBindTexture(GL_TEXTURE_2D, m_handle);

//     if (repeat)
//     {
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//         m_isRepeated = true;
//     }
//     else
//     {
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

//         const static float border_color[] { 0.0f, 0.0f, 0.0f, 1.0f };
//         glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

//         m_isRepeated = false;
//     }
//     unbind();
// }

// void Texture2D::setSmooth(bool smooth) noexcept
// {
//     glBindTexture(GL_TEXTURE_2D, m_handle);

//     if (smooth)
//     {
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         m_isSmooth = true;
//     }
//     else
//     {
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//         m_isSmooth = false;
//     }
//     unbind();
// }

// bool Texture2D::isRepeated() const noexcept
// {
//     return m_isRepeated;
// }

// bool Texture2D::isSmooth() const noexcept
// {
//     return m_isSmooth;
// }

// const glm::ivec2& Texture2D::getSize() const noexcept
// {
//     return m_size;
// }

// const std::uint8_t* Texture2D::getData() const noexcept
// {
//     return m_pixels.data();
// }

// GLuint Texture2D::getHandle() const noexcept
// {
//     return m_handle;
// }