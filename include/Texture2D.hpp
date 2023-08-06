#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

// class Texture2D final
// {
// public:
//     Texture2D() noexcept;
//     Texture2D(const Texture2D& other) = delete;
// 	Texture2D(const Texture2D&& other) = delete;
// 	Texture2D& operator = (const Texture2D& other) = delete;
// 	Texture2D& operator = (const Texture2D&& other) = delete;
//     ~Texture2D();

//     bool loadFromFile(const std::string& filepath) noexcept;
//     void bind(GLuint slot)        noexcept;
//     void unbind()                 noexcept;
//     void setRepeated(bool repeat) noexcept;
//     void setSmooth(bool smooth)   noexcept;

//     bool isRepeated()             const noexcept;
//     bool isSmooth()               const noexcept;
//     const glm::ivec2&   getSize() const noexcept;
//     const std::uint8_t* getData() const noexcept;
//     GLuint getHandle()            const noexcept;

// private:
//     std::vector<std::uint8_t> m_pixels;

//     glm::ivec2 m_size;
//     GLuint     m_handle;

//     bool m_isRepeated;
//     bool m_isSmooth;
// };
#endif