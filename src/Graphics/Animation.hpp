#ifndef ANIMATION_HPP
#define ANIMATION_HPP

struct Animation
{
	const struct Sprite2D* sprites = nullptr;
	unsigned duration = 0U;
	unsigned delay    = 0U; // in milliseconds
};

bool operator == (const Animation& a, const Animation& b) noexcept;
bool operator != (const Animation& a, const Animation& b) noexcept;

#endif // !ANIMATION_HPP