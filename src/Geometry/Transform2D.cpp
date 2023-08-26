#include "Geometry/FastSinCos.hpp"
#include "Geometry/Transform2D.hpp"

static constexpr float factor = 3.141592654f / 180.f;

Transform2D::Transform2D() noexcept :
    m_origin(0.0f, 0.0f),
    m_position(0.0f, 0.0f),
    m_rotation(0.0f),
    m_scale(1.0f, 1.0f),
    m_transformNeedUpdate (true)
{
    // Identity matrix
    m_matrix[0] = 1.f; m_matrix[4] = 0.f; m_matrix[8] = 0.f; m_matrix[12] = 0.f;
    m_matrix[1] = 0.f; m_matrix[5] = 1.f; m_matrix[9] = 0.f; m_matrix[13] = 0.f;
    m_matrix[2] = 0.f; m_matrix[6] = 0.f; m_matrix[10] = 1.f; m_matrix[14] = 0.f;
    m_matrix[3] = 0.f; m_matrix[7] = 0.f; m_matrix[11] = 0.f; m_matrix[15] = 1.f;
}

Transform2D::~Transform2D()
{
}

Transform2D* Transform2D::setPosition(float x, float y) noexcept
{
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;

    return this;
}

Transform2D* Transform2D::setPosition(const glm::vec2& position) noexcept
{
    return setPosition(position.x, position.y);
}

Transform2D* Transform2D::setRotation(float angle) noexcept
{
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;

    return this;
}

Transform2D* Transform2D::setScale(float factorX, float factorY) noexcept
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;

    return this;
}

Transform2D* Transform2D::setScale(const glm::vec2& factors) noexcept
{
    return setScale(factors.x, factors.y);
}

Transform2D* Transform2D::setOrigin(float x, float y) noexcept
{
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;

    return this;
}

Transform2D* Transform2D::setOrigin(const glm::vec2& origin) noexcept
{
    return setOrigin(origin.x, origin.y);
}

const glm::vec2& Transform2D::getPosition() const noexcept
{
    return m_position;
}

float Transform2D::getRotation() const noexcept
{
    return m_rotation;
}

const glm::vec2& Transform2D::getScale() const noexcept
{
    return m_scale;
}

const glm::vec2& Transform2D::getOrigin() const noexcept
{
    return m_origin;
}

Transform2D* Transform2D::move(float offsetX, float offsetY) noexcept
{
    return setPosition(m_position.x + offsetX, m_position.y + offsetY);
}

Transform2D* Transform2D::move(const glm::vec2& offset) noexcept
{
    return setPosition(m_position.x + offset.x, m_position.y + offset.y);
}

Transform2D* Transform2D::rotate(float angle) noexcept
{
    return setRotation(m_rotation + angle);
}

Transform2D* Transform2D::scale(float factorX, float factorY) noexcept
{
    return setScale(m_scale.x * factorX, m_scale.y * factorY);
}

Transform2D* Transform2D::scale(const glm::vec2& factor) noexcept
{
    return setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}

const float* Transform2D::getTransform() noexcept
{
    // Recompute the matrix if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * factor;
        float cosine = FastSinCos::cosine(angle);
        float sine   = FastSinCos::sine(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

        setTransform( sxc, sys, tx,
                     -sxs, syc, ty,
                      0.f, 0.f, 1.f);

        m_transformNeedUpdate = false;
    }

    return m_matrix;
}

void Transform2D::setTransform(float a00, float a01, float a02,
                               float a10, float a11, float a12,
                               float a20, float a21, float a22) noexcept
{
    m_matrix[0] = a00; m_matrix[4] = a01; m_matrix[8] = 0.f;  m_matrix[12] = a02;
    m_matrix[1] = a10; m_matrix[5] = a11; m_matrix[9] = 0.f;  m_matrix[13] = a12;
    m_matrix[2] = 0.f; m_matrix[6] = 0.f; m_matrix[10] = 1.f; m_matrix[14] = 0.f;
    m_matrix[3] = a20; m_matrix[7] = a21; m_matrix[11] = 0.f; m_matrix[15] = a22;
}
