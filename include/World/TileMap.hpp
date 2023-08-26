#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
#include <memory>

#include "rapidxml.hpp"

#include "Geometry/Vector2.hpp"
#include "Geometry/Vertex2D.hpp"

class TileMap
{
private:
	struct TilesetData
	{
		class Texture2D* pTexture = nullptr;
		std::uint32_t columns   = 0;
		std::uint32_t rows      = 0;
		std::uint32_t tileCount = 0;
		std::uint32_t firstGID  = 1;
	};

public:
	struct TilePlane
	{
		struct TileLayer
		{
			std::vector<Vertex2D>      vertices; 
			std::vector<std::uint32_t> indices;

			class Texture2D* pTexture = nullptr;
			std::uint32_t vao = 0;
			std::uint32_t vbo = 0;
		};

		std::string            m_name;
		std::vector<TileLayer> m_tileLayers;
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

		Vector2u position;
		Vector2u size;
	};

public:
	TileMap() noexcept;
	~TileMap();

	bool loadFromFile(const std::string& filename) noexcept;

	const std::vector<TilePlane>& getTilePlanes() const noexcept;
	const std::vector<Object>&    getObjects()    const noexcept;
	std::vector<const Object*>    getObjectsByName(const std::string&) const noexcept;
	std::vector<const Object*>    getObjectsByType(const std::string&) const noexcept;

	const Vector2u& getMapSizeInTiles()  const noexcept;
	Vector2u        getMapSizeInPixels() const noexcept;
	const Vector2u& getTileSize()        const noexcept;

	void draw() noexcept;
	
private:
	bool loadTilePlanes(const rapidxml::xml_node<char>* pMapNode) noexcept;
	bool loadObjects(const rapidxml::xml_node<char>* pMapNode)    noexcept;

private:
	std::vector<TilesetData>   parseTilesets(const rapidxml::xml_node<char>* pMapNode)  noexcept;
	std::vector<std::uint32_t> parseCSVstring(const rapidxml::xml_node<char>* pMapNode) noexcept;

	void unloadOnGPU(TilePlane::TileLayer& layer) noexcept;

private:
	std::vector<TilePlane> m_tilePlanes;
	std::vector<Object>    m_objects;
	std::string            m_name;
	std::string            m_collisionMask;	
	Vector2u               m_mapSize;
	Vector2u               m_tileSize;
};

#endif
