#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>

#include "Geometry/Vector2.hpp"

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

    static void bind(const Texture2D* pTexture, unsigned slot = 0) noexcept;
    void unbind() noexcept;

    void setRepeated(bool repeat) noexcept;
    void setSmooth(bool smooth)   noexcept;

    bool isRepeated()         const noexcept;
    bool isSmooth()           const noexcept;
    const Vector2i& getSize() const noexcept;
    unsigned getHandle()      const noexcept;

private:
    Vector2i m_size;
    unsigned m_handle;

    bool m_isRepeated;
    bool m_isSmooth;
};
#endif