#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

class TileMap
{
public:
	struct TilePlane
	{
		struct TileLayer
		{
			const struct Texture2D* pTexture = nullptr;

			glm::uint32_t start = 0u;
			glm::uint32_t end   = 0u;
			glm::uint32_t count = 0u;
		};

		std::string            name;
		std::vector<TileLayer> tileLayers;

		glm::uint32_t vao = 0u; // Vertex array object
		glm::uint32_t vbo = 0u; // Vertex buffer object
		glm::uint32_t ibo = 0u; // Index buffer object
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

	const std::vector<TilePlane>& getTilePlanes() const noexcept;
	const std::vector<Object>&    getObjects()    const noexcept;
	std::vector<const Object*>    getObjectsByName(const std::string&) const noexcept;
	std::vector<const Object*>    getObjectsByType(const std::string&) const noexcept;

	const glm::uvec2& getMapSizeInTiles()  const noexcept;
	const glm::uvec2& getTileSize()        const noexcept;
	glm::uvec2        getMapSizeInPixels() const noexcept;

	void draw() noexcept;

private:
    std::vector<TilePlane> m_tilePlanes;
    std::vector<Object>    m_objects;
    std::string            m_name;
    glm::uvec2             m_mapSize;
    glm::uvec2             m_tileSize;

private:
    friend class TileMapManager;
};

#endif