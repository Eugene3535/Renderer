#include <glad/glad.h>

#include "Graphics/Sprite.hpp"

Sprite::Sprite() noexcept:
	m_texture(0u),
	m_frame(0u)
{
}

Sprite::Sprite(glm::uint32_t texture) noexcept:
	m_texture(texture),
	m_frame(0u)
{
}

Sprite::Sprite(glm::uint32_t texture, glm::uint32_t frameNum) noexcept:
	m_texture(texture),
	m_frame(frameNum)
{
}

void Sprite::setTexture(glm::uint32_t texture) noexcept
{
	m_texture = texture;
}

void Sprite::setFrame(glm::uint32_t frameNum) noexcept
{
	m_frame = frameNum;
}

glm::uint32_t Sprite::getTexture() const noexcept
{
    return m_texture;
}

glm::uint32_t Sprite::getFrameNum() const noexcept
{
    return m_frame;
}

void Sprite::draw() const noexcept
{
	if(m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glDrawArrays(GL_TRIANGLE_FAN, (m_frame << 2), 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}		
}
