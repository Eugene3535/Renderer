#ifndef VERTEX2D_HPP
#define VERTEX2D_HPP

#include "Vector2.hpp"

struct Vertex2D
{
	Vertex2D() noexcept;
	Vertex2D(float x, float y, float u, float v) noexcept;
	Vertex2D(const Vector2f& thePosition, const Vector2f& theTexCoords) noexcept;

    Vector2f position;
	Vector2f texCoords;
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

inline Vertex2D::Vertex2D(const Vector2f& thePosition, const Vector2f& theTexCoords) noexcept:
	position(thePosition),
	texCoords(theTexCoords)
{
}

#endif