#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <glm/glm.hpp>

class Transform2D
{
public:
    Transform2D() noexcept;
    virtual ~Transform2D();

    Transform2D* setPosition(float x, float y) noexcept;
    Transform2D* setPosition(const glm::vec2& position) noexcept;

    Transform2D* setRotation(float angle) noexcept;

    Transform2D* setScale(float factorX, float factorY) noexcept;
    Transform2D* setScale(const glm::vec2& factors)     noexcept;

    Transform2D* setOrigin(float x, float y)        noexcept;
    Transform2D* setOrigin(const glm::vec2& origin) noexcept;

    const glm::vec2& getPosition() const noexcept;
    float            getRotation() const noexcept;
    const glm::vec2& getScale()    const noexcept;
    const glm::vec2& getOrigin()   const noexcept;

    Transform2D* move(float offsetX, float offsetY) noexcept;
    Transform2D* move(const glm::vec2& offset)      noexcept;
    Transform2D* rotate(float angle)                noexcept;

    Transform2D* scale(float factorX, float factorY) noexcept;
    Transform2D* scale(const glm::vec2& factor)      noexcept;

    const float* getTransform() noexcept;

private:
    void setTransform(float a00, float a01, float a02,
                      float a10, float a11, float a12,
                      float a20, float a21, float a22) noexcept;

private:
    float m_matrix[16];

    glm::vec2 m_origin;
    glm::vec2 m_position;         
    glm::vec2 m_scale;

    float m_rotation;       

    mutable bool m_transformNeedUpdate;       
};

#endif 
