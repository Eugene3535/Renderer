#include <glad/glad.h>

#include "Graphics/Renderer.hpp"
#include "Graphics/Primitives/Sprite.hpp"

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

void Sprite::draw(Renderer* renderer) noexcept
{
	if(m_texture)
		renderer->drawPrimitives(m_texture, GL_TRIANGLE_FAN, (m_frame << 2), 4);
}
