#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP

struct Sprite2D
{
	unsigned texture = 0U;
	unsigned frame   = 0U;
	unsigned width   = 0U;
	unsigned height  = 0U;
};

bool operator == (const Sprite2D& a, const Sprite2D& b) noexcept;
bool operator != (const Sprite2D& a, const Sprite2D& b) noexcept;

#endif // !SPRITE2D_HPP