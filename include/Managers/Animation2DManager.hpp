#ifndef ANIMATION2D_MANAGER_HPP
#define ANIMATION2D_MANAGER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Graphics/2D/Vertex2D.hpp"
#include "Managers/Base/Manager.hpp"

class Animation2DManager:
	public Manager
{
public:
	struct Animation2D
	{
		const struct Texture2D*        pTexture    = nullptr;
		const std::vector<glm::ivec2>* pFrameSizes = nullptr;

		int   startFrame = 0;   // Start frame number
		int   duration   = 0;   // Number of frames
		int   fps        = 0;   // Animation rate (frames per second)
		float delay      = 1.0f; // Delay between frames (a second by default)
	};

public:
	using SpriteSheet = std::unordered_map<std::string, Animation2D*>;

public:
	Animation2DManager() noexcept;
	~Animation2DManager();

	bool createFrame(const std::string& name, const struct Texture2D* pTexture, const glm::ivec4& frame) noexcept;
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
	void createVerticesFromFrame(const glm::ivec4& frame, const glm::vec2& ratio) noexcept;

private:
	std::unordered_map<std::string, Animation2D> m_animations;
	std::unordered_map<std::string, SpriteSheet> m_spriteSheets;

	std::vector<Vertex2D>   m_vertexBuffer;
	std::vector<glm::ivec2> m_frameSizes;

	unsigned m_vao;
	unsigned m_vbo;
};

#endif