#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <string>
#include <list>
#include <unordered_map>
#include <type_traits>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "system/NonCopyable.hpp"
#include "graphics/Vertex2D.hpp"
#include "graphics/Animation.hpp"

class SpriteManager:
	private NonCopyable
{
public:
	using SpriteSheet = std::unordered_map<std::string, Animation>;

public:
	SpriteManager() noexcept;
	~SpriteManager();

	bool createFrame(const std::string& name, const class Texture2D* texture, const glm::ivec4& frame) noexcept;
	bool createLinearAnimaton(const std::string& name, const class Texture2D* texture, std::int32_t duration, std::int32_t delay) noexcept;
	bool createGridAnimaton(const std::string& name, const class Texture2D* texture, std::int32_t columns, std::int32_t rows, std::int32_t delay) noexcept;
	bool loadSpriteSheet(const std::string& filename, const class Texture2D* texture) noexcept;

	template<class T>
	const T get(const std::string& name) const noexcept
	{
		if constexpr (std::is_same<T, Animation>::value)
		{
			auto found = m_animations.find(name);
			
			return (found != m_animations.end()) ? found->second : Animation();
		}
		else if constexpr (std::is_same<T, std::unordered_map<std::string, Animation>>::value)
		{
			auto found = m_spriteSheets.find(name);

			return (found != m_spriteSheets.end()) ? found->second : {};
		}

		return {};
	}

	void unloadOnGPU() noexcept;
	void bind(bool on) noexcept;
	void reset()       noexcept;
	void draw(const Sprite2D& sprite) const noexcept;

private:
	void createSpriteFromFrame(const glm::ivec4& frame, const glm::vec2& ratio, std::vector<Sprite2D>& sprites, std::uint32_t texture) noexcept;

private:
	std::unordered_map<std::string, Animation>   m_animations;
	std::unordered_map<std::string, SpriteSheet> m_spriteSheets;

	std::vector<Vertex2D>            m_vertexBuffer;
	std::list<std::vector<Sprite2D>> m_sprites;

	GLuint m_vao;
	GLuint m_vbo;
};

#endif