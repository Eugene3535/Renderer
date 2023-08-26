#ifndef SPRITE_HPP
#define SPRITE_HPP

class Sprite
{
public:
	Sprite() noexcept;
	~Sprite() = default;

	void setTexture(unsigned texture) noexcept;
	void setFrame(unsigned frameNum) noexcept;

	void draw() noexcept;

private:
	unsigned m_texture;
	unsigned m_frame;
};

#endif 
