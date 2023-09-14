#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Texture2D
{
	glm::uint32_t texture = 0u;
	glm::ivec2    size    = {};
};

bool LoadImageFromFile(const std::string& filepath, std::vector<glm::uint8_t>& pixels, glm::ivec2& size);
void CreateTextureFromImage(const std::vector<glm::uint8_t>& pixels, Texture2D& texture);
void SetTexture2DRepeated(glm::uint32_t texture, bool repeat);
void SetTexture2DSmooth(glm::uint32_t texture, bool smooth);

#endif