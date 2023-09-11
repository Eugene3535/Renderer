#ifndef SPRITE_HPP
#define SPRITE_HPP

class Sprite
{
public:
	Sprite() noexcept;
	~Sprite() = default;

	void setTexture(unsigned texture) noexcept;
	void setFrame(unsigned frameNum) noexcept;

	void draw(class Renderer* renderer) noexcept;

private:
	unsigned m_texture;
	unsigned m_frame;
};

#endif 
