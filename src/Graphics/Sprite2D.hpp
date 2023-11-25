#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP

#include <cstdint>

struct Sprite2D
{
	std::uint32_t texture = 0u;
	std::uint32_t frame   = 0u;
	std::uint32_t width   = 0u;
	std::uint32_t height  = 0u;
};

bool operator == (const Sprite2D& a, const Sprite2D& b) noexcept;
bool operator != (const Sprite2D& a, const Sprite2D& b) noexcept;

#endif // !SPRITE2D_HPP