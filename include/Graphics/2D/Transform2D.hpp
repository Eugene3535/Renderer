#ifndef TRANSFORM_2D_HPP
#define TRANSFORM_2D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    Transform2D* setScale(float factor)                 noexcept;

    Transform2D* setOrigin(float x, float y)        noexcept;
    Transform2D* setOrigin(const glm::vec2& origin) noexcept;

    const glm::vec2& getPosition() const noexcept;
    float            getRotation() const noexcept;
    const glm::vec2& getScale()    const noexcept;
    const glm::vec2& getOrigin()   const noexcept;

    Transform2D* move(float offsetX, float offsetY) noexcept;
    Transform2D* move(const glm::vec2& offset)       noexcept;
    Transform2D* rotate(float angle)                noexcept;

    Transform2D* scale(float factorX, float factorY) noexcept;
    Transform2D* scale(const glm::vec2& factor)      noexcept;

    const glm::mat4& getMatrix() const noexcept;

private:
    mutable glm::mat4 m_matrix;

    glm::vec2 m_origin;
    glm::vec2 m_position;         
    glm::vec2 m_scale;

    float m_rotation;       

    mutable bool m_transformNeedUpdate;       
};

#endif 
