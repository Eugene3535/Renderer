#ifndef TILE_MAP_LOADER_HPP
#define TILE_MAP_LOADER_HPP

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "rapidxml.hpp"

#include "Graphics/Vertex2D.hpp"

class TileMap
{
private:
	struct TilesetData
	{
		class Texture2D* pTexture = nullptr;
		GLuint columns   = 0;
		GLuint rows      = 0;
		GLuint tileCount = 0;
		GLuint firstGID  = 1;
	};

public:
	struct TilePlane
	{
		struct TileLayer
		{
			std::vector<Vertex2D> vertices; 
			std::vector<GLuint> indices;
			class Texture2D* pTexture = nullptr;
			GLuint vao = 0;
			GLuint vbo = 0;
		};

		std::string            name;
		std::vector<TileLayer> tileLayers;
	};

public:
	TileMap();
	~TileMap();

	bool loadFromFile(const std::string& filename);

	void draw(const GLuint shader) noexcept;
	
private:
	std::vector<TilesetData>   parseTilesets(const rapidxml::xml_node<char>* pMapNode);
	std::vector<std::uint32_t> parseCSVstring(const rapidxml::xml_node<char>* pMapNode);
	bool                       loadTilePlanes(const rapidxml::xml_node<char>* pMapNode);
	//std::string                loadCollisionMask(const rapidxml::xml_node<char>* pMapNode);

private:
	std::vector<TilePlane> m_tilePlanes;
	std::string            m_name;
	std::string            m_collisionMask;	
	glm::uvec2             m_mapSize;
	glm::uvec2             m_tileSize;
};

#endif
