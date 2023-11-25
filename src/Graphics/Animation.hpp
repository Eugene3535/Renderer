#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "graphics/Sprite2D.hpp"

struct Animation
{
	const Sprite2D* sprites  = nullptr;
	std::int32_t    duration = 0;
	std::int32_t    delay    = 0; // in milliseconds
};

bool operator == (const Animation& a, const Animation& b) noexcept;
bool operator != (const Animation& a, const Animation& b) noexcept;

#endif // !ANIMATION_HPP