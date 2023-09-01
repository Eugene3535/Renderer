#ifndef TILE_MAP_MANAGER_HPP
#define TILE_MAP_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>

#include "rapidxml.hpp"

#include "Graphics/Vertex2D.hpp"
#include "Managers/Base/Manager.hpp"

class TileMapManager:
	public Manager
{
private:
	struct TilesetData
	{
		class Texture2D* pTexture  = nullptr;
		glm::uint32_t    columns   = 0;
		glm::uint32_t    rows      = 0;
		glm::uint32_t    tileCount = 0;
		glm::uint32_t    firstGID  = 1;
	};

public:
	struct TilePlane
	{
		struct TileLayer
		{
			std::vector<Vertex2D>      vertices; 
			std::vector<glm::uint32_t> indices;

			struct Texture2D* pTexture = nullptr;
			glm::uint32_t vao = 0;
			glm::uint32_t vbo = 0;
		};

		std::string            name;
		std::vector<TileLayer> tileLayers;
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

		glm::uvec2 position;
		glm::uvec2 size;
	};

public:
	struct TileMap
	{
		std::vector<TilePlane> tilePlanes;
		std::vector<Object>    objects;
		std::string            name;
		std::string            collisionMask;	
		glm::uvec2             mapSize;
		glm::uvec2             tileSize;
	};

public:
	TileMapManager() noexcept;
	~TileMapManager();

	const TileMap* loadFromFile(const std::string& filename) noexcept;

	void draw() noexcept;
	
private:
	bool loadTilePlanes(const rapidxml::xml_node<char>* pMapNode) noexcept;
	bool loadObjects(const rapidxml::xml_node<char>* pMapNode)    noexcept;

private:
	std::vector<TilesetData>   parseTilesets(const rapidxml::xml_node<char>* pMapNode)  noexcept;
	std::vector<glm::uint32_t> parseCSVstring(const rapidxml::xml_node<char>* pMapNode) noexcept;

	void unloadOnGPU(TilePlane::TileLayer& layer) noexcept;

private:
	std::vector<std::unique_ptr<TileMap>> m_tileMaps;
};

#endif
