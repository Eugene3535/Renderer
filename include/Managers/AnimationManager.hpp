#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include "Utils/Defines.hpp"

struct Animation
{
	class Texture2D* pTexture = nullptr;
	unsigned         vao = 0;
	unsigned         vbo = 0;
	unsigned         duration = 0;
	float            rate     = 0.0f;
	float            delay    = 1.0f;
};

class AnimationManager
{
public:
	using SpriteSheet = std::unordered_map<std::string, Animation*>;

public:
    AnimationManager() noexcept;
    ~AnimationManager();

	 bool create(const char* name, class Texture2D* pTexture, const glm::ivec4& frame) noexcept;
	 bool create(const char* name, class Texture2D* pTexture, const glm::ivec4& startFrame, int duration, float fps, float delay = 1.0f) noexcept;
	 bool create(const char* name, class Texture2D* pTexture, int duration, float fps, float delay = 1.0f) noexcept;
	 bool create(const char* name, class Texture2D* pTexture, int columns, int rows, float fps, float delay = 1.0f) noexcept;
	 bool loadSpriteSheet(const char* filename, class Texture2D* pTexture) noexcept;

    const Animation*   getAnimation(const std::string& name)   const noexcept;
	const SpriteSheet* getSpriteSheet(const std::string& name) const noexcept;

private:
	std::unordered_map<std::string, Animation>   m_animMap;
	std::unordered_map<std::string, SpriteSheet> m_spriteSheets;
};

#endif