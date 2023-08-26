#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Geometry/Vertex2D.hpp"
#include "Geometry/Rect.hpp"

class SpriteManager
{
public:
	struct Animation2D
	{
		const struct Texture2D* pTexture = nullptr;

		unsigned startFrame = 0u;   // Start frame number
		unsigned duration   = 0u;   // Number of frames
		unsigned fps        = 0u;   // Animation rate (frames per second)
		float    delay      = 1.0f; // Delay between frames (a second by default)
	};

public:
	using SpriteSheet = std::unordered_map<std::string, Animation2D*>;

public:
	SpriteManager() noexcept;
	~SpriteManager();

	bool createFrame(const std::string& name, const struct Texture2D* pTexture, const IntRect& frame) noexcept;
	bool createLinearAnimaton(const std::string& name, const struct Texture2D* pTexture, int duration, int fps, float delay = 1.0f) noexcept;
	bool createGridAnimaton(const std::string& name, const struct Texture2D* pTexture, int columns, int rows, int fps, float delay = 1.0f) noexcept;
	bool loadSpriteSheet(const std::string& filename, const struct Texture2D* pTexture) noexcept;

	const Animation2D* getAnimation(const std::string& name)   const noexcept;
	const SpriteSheet* getSpriteSheet(const std::string& name) const noexcept;

	void unloadOnGPU() noexcept;
	void bind()        noexcept;
	void unbind()      noexcept;
	void release()     noexcept;

private:
	unsigned createVerticesFromFrame(const IntRect& frame, const Vector2f& ratio) noexcept;

private:
	std::unordered_map<std::string, Animation2D> m_animations;
	std::unordered_map<std::string, SpriteSheet> m_spriteSheets;

	std::vector<Vertex2D> m_vertexBuffer;
	std::vector<Vector2i> m_spriteSizes;

	unsigned m_vao;
	unsigned m_vbo;
	unsigned m_amountOfSprites;
};

#endif