#include "TileMap.hpp"

#include <glad/glad.h>

#include <sstream>
#include <iostream>
#include <algorithm>

#include "rapidxml_utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Files.hpp"
#include "Utils/Defines.hpp"

#include "Managers/AssetManager.hpp"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

bool TileMap::loadFromFile(const std::string& filename)
{
	std::string filepath = FileUtils::getPathToFile(filename);

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
		auto pImage = pTilesetNode->first_node("image");
		auto pSource = pImage->first_attribute("source");

		std::string texName = pSource ? pSource->value() : std::string();

		if (texName.empty())
			continue;

		std::size_t last_slash_pos = texName.find_last_of('/');

		if (last_slash_pos != std::string::npos)	
			texName.erase(0, last_slash_pos + 1);
		
		auto pTileset = AssetManager::get<Texture2D>(texName);

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

std::vector<unsigned> TileMap::parseCSVstring(const rapidxml::xml_node<char>* pDataNode)
{
	std::string data(pDataNode->value());

	std::size_t amount = std::count_if(data.begin(), data.end(), [](char c){ return c == ','; }) + 1;
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<unsigned> parsed_layer;
	parsed_layer.reserve(amount);

	std::stringstream sstream(data);
	{
		unsigned tile_num = 0;

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

	const unsigned map_width   = pMapW  ? std::atoi(pMapW->value())  : 0;
	const unsigned map_height  = pMapH  ? std::atoi(pMapH->value())  : 0;
	const unsigned tile_width  = pTileW ? std::atoi(pTileW->value()) : 0;
	const unsigned tile_height = pTileH ? std::atoi(pTileH->value()) : 0;

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

		auto pDataNode = pLayerNode->first_node("data");

		if( ! pDataNode )
			continue;

		std::vector<unsigned> parsed_layer = parseCSVstring(pDataNode);

		std::size_t non_zero_tile_count = std::count_if(parsed_layer.begin(), parsed_layer.end(),
			[](unsigned n) { return n > 0; });

		auto& plane = m_tilePlanes.emplace_back();
		plane.m_name = name;

		for (unsigned y = 0; y < map_height; ++y)
			for (unsigned x = 0; x < map_width; ++x)
			{
				unsigned tile_id = parsed_layer[y * map_width + x];

				if (tile_id)
				{
					// Need to find from what tileset this tile is
					auto current_tileset = std::find_if(tilesets.begin(), tilesets.end(),
						[tile_id](const TilesetData& ts)
						{
							return tile_id >= ts.firstGID && tile_id <= ts.firstGID + ts.tileCount;
						});

					// Is there associated tile layer for this tileset?
					auto current_layer = std::find_if(plane.m_tileLayers.begin(), plane.m_tileLayers.end(),
						[&current_tileset](const TileMap::TilePlane::TileLayer& layer)
						{
							return current_tileset->pTexture == layer.pTexture;
						});

					TileMap::TilePlane::TileLayer* pLayer = nullptr;
					// If not - we will create it
					if (current_layer != plane.m_tileLayers.end())
						pLayer = std::addressof(*current_layer);
					else
					{
						pLayer = std::addressof(plane.m_tileLayers.emplace_back());
						pLayer->pTexture = current_tileset->pTexture;
					}

					// Find the sequence tile number in this tileset
					unsigned tile_num = tile_id - current_tileset->firstGID;

					// left-top coords of the tile in texture grid
					unsigned Y = (tile_num >= current_tileset->columns) ? tile_num / current_tileset->columns : 0;
					unsigned X = tile_num % current_tileset->columns;
					glm::vec2 tex_coords(X * tile_width, Y * tile_height);

					int width = pLayer->pTexture->getSize().x;
					int height = pLayer->pTexture->getSize().y;

					float left   = tex_coords.x / width;
					float top    = tex_coords.y / height;
					float right  = (tex_coords.x + tile_width)  / width;
					float bottom = (tex_coords.y + tile_height) / height;

					unsigned index = static_cast<unsigned>(pLayer->vertices.size());

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
		for(auto& layer : plane.m_tileLayers)
		{
			glGenVertexArrays(1, &layer.vao);
	 		glGenBuffers(1, &layer.vbo);

			glBindVertexArray(layer.vao);

			glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * layer.vertices.size(), layer.vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, u));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);	

			layer.vertices.clear();
		}

	return ! m_tilePlanes.empty();
}

void TileMap::draw(const unsigned shader) noexcept
{
	glUseProgram(shader);

	for(auto& plane : m_tilePlanes)
		for(auto& layer : plane.m_tileLayers)
		{
			layer.pTexture->bind(layer.pTexture);
			glBindVertexArray(layer.vao);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned>(layer.indices.size()), GL_UNSIGNED_INT, layer.indices.data());
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	glUseProgram(0);
}