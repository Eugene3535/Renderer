#ifndef VERTEX2D_HPP
#define VERTEX2D_HPP

#include <glm/glm.hpp>

struct Vertex2D
{
	Vertex2D() noexcept;
	Vertex2D(float x, float y, float u, float v) noexcept;
	Vertex2D(const glm::vec2& thePosition, const glm::vec2& theTexCoords) noexcept;

    glm::vec2 position;
	glm::vec2 texCoords;
};

inline Vertex2D::Vertex2D() noexcept: 
	position(),
	texCoords()
{
}

inline Vertex2D::Vertex2D(float x, float y, float u, float v) noexcept: 
	position(x, y),
	texCoords(u, v)
{
}

inline Vertex2D::Vertex2D(const glm::vec2& thePosition, const glm::vec2& theTexCoords) noexcept:
	position(thePosition),
	texCoords(theTexCoords)
{
}

#endif // !VERTEX2D_HPP