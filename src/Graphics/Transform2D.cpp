#include "Graphics/Transform2D.hpp"

#include <cmath>

#include <glm/gtc/type_ptr.hpp>

static constexpr float factor = 3.141592654f / 180.f;

Transform2D::Transform2D() noexcept :
    m_matrix(glm::identity<glm::mat4>()),
    m_origin(0.0f, 0.0f),
    m_position(0.0f, 0.0f),
    m_scale(1.0f, 1.0f),
    m_rotation(0.0f),
    m_transformNeedUpdate(true)
{
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

Transform2D* Transform2D::setScale(float factor) noexcept
{
    return setScale(factor, factor);
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

const glm::mat4& Transform2D::getMatrix() const noexcept
{
    // Recompute the matrix if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * factor;
        float cosine = cos(angle);
        float sine   = sin(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

        auto m = glm::value_ptr(m_matrix);

        m[0] = sxc;  m[4] = sys; m[8] = 0.f;  m[12] = tx;
        m[1] = -sxs; m[5] = syc; m[9] = 0.f;  m[13] = ty;
        m[2] = 0.f;  m[6] = 0.f; m[10] = 1.f; m[14] = 0.f;
        m[3] = 0.f;  m[7] = 0.f; m[11] = 0.f; m[15] = 1.f;

        m_transformNeedUpdate = false;
    }

    return m_matrix;
}
