#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "rapidxml.hpp"

#include "Graphics/Vertex2D.hpp"

class TileMap
{
private:
	struct TilesetData
	{
		class Texture2D* pTexture = nullptr;
		unsigned columns   = 0;
		unsigned rows      = 0;
		unsigned tileCount = 0;
		unsigned firstGID  = 1;
	};

public:
	struct TilePlane
	{
		struct TileLayer
		{
			std::vector<Vertex2D> vertices; 
			std::vector<unsigned> indices;
			class Texture2D* pTexture = nullptr;
			unsigned vao = 0;
			unsigned vbo = 0;
		};

		std::string            m_name;
		std::vector<TileLayer> m_tileLayers;
	};

public:
	TileMap();
	~TileMap();

	bool loadFromFile(const std::string& filename);

	void draw(const unsigned shader) noexcept;
	
private:
	std::vector<TilesetData> parseTilesets(const rapidxml::xml_node<char>* pMapNode);
	std::vector<unsigned>    parseCSVstring(const rapidxml::xml_node<char>* pMapNode);
	bool                     loadTilePlanes(const rapidxml::xml_node<char>* pMapNode);

private:
	std::vector<TilePlane> m_tilePlanes;
	std::string            m_name;
	std::string            m_collisionMask;	
	glm::uvec2             m_mapSize;
	glm::uvec2             m_tileSize;
};

#endif
