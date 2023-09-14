#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

class TileMap
{
public:
	struct TileLayer
	{
		std::string name;

		glm::uint32_t texture = 0u; // Texture handle
		glm::uint32_t count   = 0u; // Number of indices to render
		glm::uint32_t vao     = 0u; // Vertex array object
		glm::uint32_t vbo     = 0u; // Vertex buffer object
		glm::uint32_t ebo     = 0u; // Element buffer object
	};

public:
	struct Object
	{
		struct Property
		{
			std::string name;
			std::string type;
			std::string value;
		};

		std::string name;
		std::string type;

		std::vector<Property> properties;

		glm::ivec2 position;
		glm::ivec2 size;
	};

public:
	TileMap() noexcept;
	~TileMap();

	const std::vector<TileLayer>& getTileLayers() const noexcept;
	const std::vector<Object>&    getObjects()    const noexcept;
	std::vector<const Object*>    getObjectsByName(const std::string&) const noexcept;
	std::vector<const Object*>    getObjectsByType(const std::string&) const noexcept;

	const glm::uvec2& getMapSizeInTiles()  const noexcept;
	const glm::uvec2& getTileSize()        const noexcept;
	glm::uvec2        getMapSizeInPixels() const noexcept;

	void draw(const TileLayer& layer) const noexcept;

private:
    std::vector<TileLayer> m_tileLayers;
    std::vector<Object>    m_objects;
    std::string            m_name;
    glm::uvec2             m_mapSize;
    glm::uvec2             m_tileSize;

private:
    friend class TileMapManager;
};

#endif