#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Texture2D
{
public:
    Texture2D() noexcept;
    Texture2D(const Texture2D& other) = delete;
	Texture2D(const Texture2D&& other) = delete;
	Texture2D& operator = (const Texture2D& other) = delete;
	Texture2D& operator = (const Texture2D&& other) = delete;
    ~Texture2D();

    bool loadFromFile(const std::string& filepath) noexcept;
    void bind(GLuint slot = 0)    noexcept;
    void unbind()                 noexcept;
    void setRepeated(bool repeat) noexcept;
    void setSmooth(bool smooth)   noexcept;

    bool isRepeated()           const noexcept;
    bool isSmooth()             const noexcept;
    const glm::ivec2& getSize() const noexcept;
    GLuint getHandle()          const noexcept;

private:
    glm::ivec2 m_size;
    GLuint     m_handle;

    bool m_isRepeated;
    bool m_isSmooth;
};
#endif