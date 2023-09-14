#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Transform2D.hpp"

class Sprite:
	public Transform2D
{
public:
	Sprite() noexcept;
	~Sprite() = default;

	void setTexture(glm::uint32_t texture) noexcept;
	void setFrame(glm::uint32_t frameNum) noexcept;

	glm::uint32_t getTexture() const noexcept;
	glm::uint32_t getFrameNum() const noexcept;

	void draw() const noexcept;

private:
	glm::uint32_t m_texture;
	glm::uint32_t m_frame;
};

#endif 
