#include "Loaders/TileMap.hpp"

#include <sstream>
#include <iostream>
#include <algorithm>

#include "rapidxml_utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Files.hpp"
#include "Utils/Defines.hpp"

#include "Loaders/AssetLoader.hpp"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

bool TileMap::loadFromFile(const std::string& filename)
{
	std::string filepath = FileUtils::getPathToFile(filename, "levels");

	if(filepath.empty())
		return false;

	auto pDocument = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	pDocument->parse<0>(xmlFile.data());
	const rapidxml::xml_node<char>* pMapNode = pDocument->first_node("map");

	if( ! pMapNode )
		return false;

	m_name = filename;

	if( ! loadTilePlanes(pMapNode) )
		return false;

	return true;
}

std::vector<TileMap::TilesetData> TileMap::parseTilesets(const rapidxml::xml_node<char>* pMapNode)
{
	std::vector<TilesetData> tilesets;

	for (auto pTilesetNode = pMapNode->first_node("tileset");
		      pTilesetNode != nullptr;
		      pTilesetNode = pTilesetNode->next_sibling("tileset"))
	{
		auto pTexName       = pTilesetNode->first_attribute("name");
		std::string texName = pTexName ?  pTexName->value() : std::string();

		const auto pTileset = AssetLoader::getTexture(texName);

		if( pTileset == nullptr )
			continue;

		TilesetData& ts = tilesets.emplace_back();

		auto pTileCount = pTilesetNode->first_attribute("tilecount");
		auto pColumns   = pTilesetNode->first_attribute("columns");
		auto pFirstGID  = pTilesetNode->first_attribute("firstgid");

		ts.pTexture  = pTileset;
		ts.tileCount = pTileCount ? std::stoul(pTileCount->value()) : 0;
		ts.columns   = pColumns ? std::stoul(pColumns->value()) : 0;
		ts.rows      = ( ! pTileCount || ! pColumns ) ? 0 : ts.tileCount / ts.columns;
		ts.firstGID  = pFirstGID ? std::stoul(pFirstGID->value()) : 0;
	}

	return tilesets;
}

std::vector<GLuint> TileMap::parseCSVstring(const rapidxml::xml_node<char>* pDataNode)
{
	std::string data(pDataNode->value());

	std::size_t amount = std::count_if(data.begin(), data.end(), [](char c){ return c == ','; }) + 1;
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<GLuint> parsed_layer;
	parsed_layer.reserve(amount);

	std::stringstream sstream(data);
	{
		GLuint tile_num = 0;

		while (sstream >> tile_num)
			parsed_layer.push_back(tile_num);
	}

	return parsed_layer;
}

bool TileMap::loadTilePlanes(const rapidxml::xml_node<char>* pMapNode)
{
	std::vector<TilesetData> tilesets = parseTilesets(pMapNode);

	if(tilesets.empty())
		return false;

	auto pMapW  = pMapNode->first_attribute("width");
	auto pMapH  = pMapNode->first_attribute("height");
	auto pTileW = pMapNode->first_attribute("tilewidth");
	auto pTileH = pMapNode->first_attribute("tileheight");

	const GLint map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const GLint map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const GLint tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const GLint tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

#ifdef DEBUG
	if( ! map_width || ! map_height || ! tile_width || ! tile_height ) 
		return false;
#endif

	m_mapSize  = glm::ivec2(map_width, map_height);
	m_tileSize = glm::ivec2(tile_width, tile_height);

	for (auto pLayerNode = pMapNode->first_node("layer");
		      pLayerNode != nullptr;
		      pLayerNode = pLayerNode->next_sibling("layer"))
	{
		auto pName       = pLayerNode->first_attribute("name");
		std::string name = pName ? pName->value() : std::string();

		if(name.empty())
			continue;

		if (name == "Collision mask")
		{
			//pTileMap->collisionMask = loadCollisionMask(pMapNode); 
			continue;
		}

		auto pDataNode = pLayerNode->first_node("data");

		if( ! pDataNode )
			continue;

		std::vector<GLuint> parsed_layer = parseCSVstring(pDataNode);

		std::size_t non_zero_tile_count = std::count_if(parsed_layer.begin(), parsed_layer.end(),
			[](GLuint n) { return n > 0; });

		auto& plane = m_tilePlanes.emplace_back();
		plane.name = name;

		for (GLint y = 0; y < map_height; ++y)
			for (GLint x = 0; x < map_width; ++x)
			{
				GLuint tile_id = parsed_layer[y * map_width + x];

				if (tile_id)
				{
					// Need to find from what tileset this tile is
					auto current_tileset = std::find_if(tilesets.begin(), tilesets.end(),
						[tile_id](const TilesetData& ts)
						{
							return tile_id >= ts.firstGID && tile_id <= ts.firstGID + ts.tileCount;
						});

					// Is there associated tile layer for this tileset?
					auto current_layer = std::find_if(plane.tileLayers.begin(), plane.tileLayers.end(),
						[&current_tileset](const TileMap::TilePlane::TileLayer& layer)
						{
							return current_tileset->pTexture == layer.pTexture;
						});

					TileMap::TilePlane::TileLayer* pLayer = nullptr;
					// If not - we will create it
					if (current_layer != plane.tileLayers.end())
						pLayer = std::addressof(*current_layer);
					else
					{
						pLayer = std::addressof(plane.tileLayers.emplace_back());
						pLayer->pTexture = current_tileset->pTexture;
					}

					// Find the sequence tile number in this tileset
					GLuint tile_num = tile_id - current_tileset->firstGID;

					// left-top coords of the tile in texture grid
					GLuint Y = (tile_num >= current_tileset->columns) ? tile_num / current_tileset->columns : 0;
					GLuint X = tile_num % current_tileset->columns;
					glm::vec2 tex_coords(X * tile_width, Y * tile_height);

					float left   = tex_coords.x / pLayer->pTexture->width;
					float top    = tex_coords.y / pLayer->pTexture->height;
					float right  = (tex_coords.x + tile_width)  / pLayer->pTexture->width;
					float bottom = (tex_coords.y + tile_height) / pLayer->pTexture->height;

					GLuint index = static_cast<GLuint>(pLayer->vertices.size());

					// Quad
					pLayer->vertices.emplace_back(x * tile_width,              y * tile_height + tile_height, left,  bottom, Color::White);
					pLayer->vertices.emplace_back(x * tile_width + tile_width, y * tile_height + tile_height, right, bottom, Color::White);
					pLayer->vertices.emplace_back(x * tile_width + tile_width, y * tile_height              , right, top   , Color::White);
					pLayer->vertices.emplace_back(x * tile_width,              y * tile_height              , left,  top   , Color::White);

					// 1-st triangle
					pLayer->indices.push_back(index);
					pLayer->indices.push_back(index + 1);
					pLayer->indices.push_back(index + 2);

					// 2-nd triangle
					pLayer->indices.push_back(index);
					pLayer->indices.push_back(index + 2);
					pLayer->indices.push_back(index + 3);			
				}
			}
	}

	for(auto& plane : m_tilePlanes)
		for(auto& layer : plane.tileLayers)
		{
			glGenVertexArrays(1, &layer.vao);
	 		glGenBuffers(1, &layer.vbo);

			glBindVertexArray(layer.vao);

			glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * layer.vertices.size(), layer.vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);	

			layer.vertices.clear();
		}

	return ! m_tilePlanes.empty();
}

void TileMap::draw(const GLuint shader) noexcept
{
	glUseProgram(shader);

	for(auto& plane : m_tilePlanes)
		for(auto& layer : plane.tileLayers)
		{
			glBindTexture(GL_TEXTURE_2D, layer.pTexture->id);
			glBindVertexArray(layer.vao);
			glDrawElements(GL_TRIANGLES, static_cast<GLuint>(layer.indices.size()), GL_UNSIGNED_INT, layer.indices.data());
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	glUseProgram(0);
}