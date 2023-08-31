#include <glad/glad.h>

#include "Graphics/Renderer.hpp"
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

void Sprite::draw(Renderer* renderer) noexcept
{
	Renderer::States states{0};
	states.texture = m_texture;
	states.mode = GL_TRIANGLE_FAN;
	states.firstVertex = (m_frame << 2);
	states.count = 4;

	renderer->drawStates(states);
}
