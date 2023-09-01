#ifndef VERTEX3D_HPP
#define VERTEX3D_HPP

#include <glm/glm.hpp>

struct Vertex3D
{
	Vertex3D() noexcept;
	Vertex3D(const glm::vec3& thePosition, const glm::vec3& theNormals, const glm::vec2& theTexCoords) noexcept;
    Vertex3D(const float* data) noexcept;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

inline Vertex3D::Vertex3D() noexcept: 
	position(),
    normal(),
	uv()
{
}

inline Vertex3D::Vertex3D(const glm::vec3& thePosition, const glm::vec3& theNormals, const glm::vec2& theTexCoords) noexcept:
	position(thePosition),
    normals(theNormals),
	uv(theTexCoords)
{
}

inline Vertex3D::Vertex3D(const float* data) noexcept:
	position(data[0], data[1], data[2]),
    normals(data[3], data[4], data[5]),
	uv(data[6], data[7])
{
}

#endif