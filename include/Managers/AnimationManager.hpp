#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <string>

#include "Utils/Defines.hpp"
#include "Utils/Rect.hpp"

struct Animation
{
	const class Texture2D* pTexture = nullptr;
	unsigned vao = 0;
	unsigned vbo = 0;
	unsigned duration = 0;
	float    rate     = 0.0f;
	float    delay    = 1.0f;
};

class AnimationManager
{
public:
	using SpriteSheet = std::unordered_map<std::string, Animation*>;

public:
    AnimationManager() noexcept;
    ~AnimationManager();

	 static const Animation* create(const char* name, const class Texture2D* pTexture, const IntRect& rect) noexcept;
	 static const Animation* create(const char* name, const class Texture2D* pTexture, int duration, float fps, float delay = 1.0f) noexcept;
	 static const Animation* create(const char* name, const class Texture2D* pTexture, int columns, int rows, float fps, float delay = 1.0f) noexcept;
	 static const SpriteSheet* loadSpriteSheet(const std::string& filename, const class Texture2D* pTexture) noexcept;

     static const Animation*   getAnimation(const std::string& name)   noexcept;
	 static const SpriteSheet* getSpriteSheet(const std::string& name) noexcept;

private:
	static void unloadOnGPU(const class Vertex2D* vertices, Animation& anim) noexcept;

private:
	std::unordered_map<std::string, Animation>   m_animations;
	std::unordered_map<std::string, SpriteSheet> m_spriteSheets;

private:
	static AnimationManager* m_pInstance;
};

#endif