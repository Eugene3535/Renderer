#include <glad/glad.h>

#include "graphics/2D/Sprite2D.hpp"

Sprite2D::Sprite2D() noexcept:
	m_texture(0u),
	m_frame(0u)
{
}

Sprite2D::Sprite2D(glm::uint32_t texture) noexcept:
	m_texture(texture),
	m_frame(0u)
{
}

Sprite2D::Sprite2D(glm::uint32_t texture, glm::uint32_t frameNum) noexcept:
	m_texture(texture),
	m_frame(frameNum)
{
}

void Sprite2D::setTexture(glm::uint32_t texture) noexcept
{
	m_texture = texture;
}

void Sprite2D::setFrame(glm::uint32_t frameNum) noexcept
{
	m_frame = frameNum;
}

glm::uint32_t Sprite2D::getTexture() const noexcept
{
    return m_texture;
}

glm::uint32_t Sprite2D::getFrameNum() const noexcept
{
    return m_frame;
}

void Sprite2D::draw() const noexcept
{
	if(m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glDrawArrays(GL_TRIANGLE_FAN, (m_frame << 2), 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}		
}
