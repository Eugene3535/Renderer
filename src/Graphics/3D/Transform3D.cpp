#include <cstring>

#include "extmath/ExtMath.hpp"
#include "graphics/3D/Transform3D.hpp"

const Transform3D Transform3D::Identity;

Transform3D::Transform3D() noexcept: // Identity matrix
    m_matrix{ 1.f, 0.f, 0.f, 0.f,
              0.f, 1.f, 0.f, 0.f,
              0.f, 0.f, 1.f, 0.f,
              0.f, 0.f, 0.f, 1.f }
{
}

Transform3D::Transform3D(const float* matrix) noexcept:
    Transform3D()
{
    std::memcpy(m_matrix, matrix, (sizeof(float) << 4));
}

Transform3D::~Transform3D()
{
}