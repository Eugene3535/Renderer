#include <glad/glad.h>

#include "Graphics/Sprite.hpp"

Sprite::Sprite() noexcept:
	m_texture(0u),
	m_frame(0u)
{
}

void Sprite::setTexture(unsigned texture) noexcept
{
	m_texture = texture;
}

void Sprite::setFrame(unsigned frameNum) noexcept
{
	m_frame = frameNum;
}

void Sprite::draw() noexcept
{
	if(m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glDrawArrays(GL_TRIANGLE_FAN, (m_frame << 2), 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
