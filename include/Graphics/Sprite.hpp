#ifndef SPRITE_HPP
#define SPRITE_HPP

class Sprite
{
public:
	Sprite() noexcept;
	~Sprite();

	void draw(const class Shader* shader) noexcept;

private:
	const class Animation* m_pAnimation;
};


#endif 
