#include <glad/glad.h>

#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>

#include "rapidxml_utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Files.hpp"
#include "Utils/Defines.hpp"
#include "Managers/AssetManager.hpp"
#include "Managers/TileMapManager.hpp"

TileMapManager::TileMapManager() noexcept
{
}

TileMapManager::~TileMapManager()
{
}

const TileMap* TileMapManager::loadFromFile(const std::string& filename) noexcept
{
	const auto loaded = get(filename);
	
	if(loaded)
		return loaded;

	const std::string filepath = FileUtils::getPathToFile(filename);

	if(filepath.empty())
		return nullptr;

	auto pDocument = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	pDocument->parse<0>(xmlFile.data());
	const rapidxml::xml_node<char>* pMapNode = pDocument->first_node("map");

	if( ! pMapNode )
		return nullptr;

	auto& pTileMap = m_tileMaps.emplace_back(std::make_unique<TileMap>());
	pTileMap->m_name = filename;

	if ( ! (loadTilePlanes(pMapNode) && loadObjects(pMapNode)) )
	{
		m_tileMaps.pop_back();

		return nullptr;
	}

	return pTileMap.get();
}

const TileMap* TileMapManager::get(const std::string& filename) noexcept
{
	for(const auto& tilemap : m_tileMaps)
		if(tilemap->m_name == filename)
			return tilemap.get();

    return nullptr;
}

void TileMapManager::clear() noexcept
{
	m_tileMaps.clear();
}

bool TileMapManager::loadTilePlanes(const rapidxml::xml_node<char>* pMapNode) noexcept
{
	std::vector<TilesetData> tilesets = parseTilesets(pMapNode);

	if (tilesets.empty())
		return false;

	auto pMapW  = pMapNode->first_attribute("width");
	auto pMapH  = pMapNode->first_attribute("height");
	auto pTileW = pMapNode->first_attribute("tilewidth");
	auto pTileH = pMapNode->first_attribute("tileheight");

	const glm::uint32_t map_width   = pMapW  ? std::strtol(pMapW->value(),  nullptr, 10) : 0u;
	const glm::uint32_t map_height  = pMapH  ? std::strtol(pMapH->value(),  nullptr, 10) : 0u;
	const glm::uint32_t tile_width  = pTileW ? std::strtol(pTileW->value(), nullptr, 10) : 0u;
	const glm::uint32_t tile_height = pTileH ? std::strtol(pTileH->value(), nullptr, 10) : 0u;

	auto pTileMap = m_tileMaps.back().get();

	pTileMap->m_mapSize  = { map_width, map_height };
	pTileMap->m_tileSize = { tile_width, tile_height };

	std::unordered_map<TileMap::TilePlane::TileLayer*, std::vector<Vertex2D>> vertexMap;

	for (auto pLayerNode = pMapNode->first_node("layer");
			  pLayerNode != nullptr;
			  pLayerNode = pLayerNode->next_sibling("layer"))
	{
		auto pName = pLayerNode->first_attribute("name");
		std::string name = pName ? pName->value() : std::string();

		if (name.empty())
			continue;

		auto pDataNode = pLayerNode->first_node("data");

		if (!pDataNode)
			continue;

		std::vector<glm::uint32_t> parsed_layer = parseCSVstring(pDataNode);

		auto& plane = pTileMap->m_tilePlanes.emplace_back();
		plane.name = name;
// Reserve memory for all layers in plane( fix for invalid pointer after std::vector::resize() )
		{
			auto cmp = [](const glm::uvec2& a, const glm::uvec2& b) { return a.x < b.x && a.y < b.y; };

			std::set<glm::uvec2, decltype(cmp)> bounds(cmp);

			for (auto tileNum : parsed_layer)
			{
				// Need to find from what tileset this tile is
				if (tileNum)
				{
					auto from = std::find_if(tilesets.cbegin(), tilesets.cend(),
						[tileNum](const TilesetData& ts)
						{
							return tileNum >= ts.firstGID && tileNum <= ts.firstGID + ts.tileCount;
						});
					// Find the range of this tileset
					bounds.insert(glm::uvec2(from->firstGID, from->firstGID + from->tileCount));
				}
			}

			plane.tileLayers.reserve(bounds.size());
		}

		for (glm::uint32_t y = 0u; y < map_height; ++y)
			for (glm::uint32_t x = 0u; x < map_width; ++x)
			{
				glm::uint32_t tile_id = parsed_layer[y * map_width + x];

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

					auto& vertices = vertexMap[pLayer];

// Find the sequence tile number in this tileset
					glm::uint32_t tile_num = tile_id - current_tileset->firstGID;

// Left-top coords of the tile in texture grid
					glm::uint32_t Y = (tile_num >= current_tileset->columns) ? tile_num / current_tileset->columns : 0;
					glm::uint32_t X = tile_num % current_tileset->columns;

					glm::uint32_t offsetX = X * tile_width;
					glm::uint32_t offsetY = Y * tile_height;

					auto ratio = 1.0f / glm::vec2(pLayer->pTexture->width, pLayer->pTexture->height);
// Texture coords
					float left   = offsetX * ratio.x;
					float top    = offsetY * ratio.y;
					float right  = (offsetX + tile_width) * ratio.x;
					float bottom = (offsetY + tile_height) * ratio.y;
// Vertex coords
					glm::vec2 leftBottom  = { x * tile_width,              y * tile_height + tile_height };
					glm::vec2 rightBootom = { x * tile_width + tile_width, y * tile_height + tile_height };
					glm::vec2 rightTop    = { x * tile_width + tile_width, y * tile_height };
					glm::vec2 leftTop     = { x * tile_width,              y * tile_height };
// Index stride
					glm::uint32_t index = static_cast<glm::uint32_t>(vertices.size());
// Quad
					vertices.emplace_back(leftBottom.x, leftBottom.y, left, bottom);
					vertices.emplace_back(rightBootom.x, rightBootom.y, right, bottom);
					vertices.emplace_back(rightTop.x, rightTop.y, right, top);
					vertices.emplace_back(leftTop.x, leftTop.y, left, top);
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

	unloadOnGPU(vertexMap);

	return !pTileMap->m_tilePlanes.empty();
}

bool TileMapManager::loadObjects(const rapidxml::xml_node<char>* pMapNode) noexcept
{
	auto pTileMap = m_tileMaps.back().get();

	for (auto pObjectGroupNode = pMapNode->first_node("objectgroup");
			  pObjectGroupNode != nullptr;
			  pObjectGroupNode = pObjectGroupNode->next_sibling("objectgroup"))
	{
		for (auto pObjectNode = pObjectGroupNode->first_node("object");
				  pObjectNode != nullptr;
				  pObjectNode = pObjectNode->next_sibling("object"))
		{
			auto& tme_object = pTileMap->m_objects.emplace_back();

			for (auto pAttr = pObjectNode->first_attribute(); pAttr != nullptr; pAttr = pAttr->next_attribute())
			{
				if (strcmp(pAttr->name(), "x")      == 0) { tme_object.position.x = std::atoi(pAttr->value()); continue; }
				if (strcmp(pAttr->name(), "y")      == 0) { tme_object.position.y = std::atoi(pAttr->value()); continue; }
				if (strcmp(pAttr->name(), "width")  == 0) { tme_object.size.x     = std::atoi(pAttr->value()); continue; }
				if (strcmp(pAttr->name(), "height") == 0) { tme_object.size.y     = std::atoi(pAttr->value()); continue; }

				if (strcmp(pAttr->name(), "name")  == 0) tme_object.name = pAttr->value();
				if (strcmp(pAttr->name(), "class") == 0) tme_object.type = pAttr->value();
			}

			if (const auto pPropertiesNode = pObjectNode->first_node("properties"); pPropertiesNode != nullptr)
			{
				for (auto pPropertyNode = pPropertiesNode->first_node("property");
					      pPropertyNode != nullptr;
					      pPropertyNode = pPropertyNode->next_sibling("property"))
				{
					auto& prop = tme_object.properties.emplace_back();

					for (auto pAttr = pPropertyNode->first_attribute(); pAttr != nullptr; pAttr = pAttr->next_attribute())
					{
						if (strcmp(pAttr->name(), "name")  == 0) { prop.name  = pAttr->value(); continue; }
						if (strcmp(pAttr->name(), "type")  == 0) { prop.type  = pAttr->value(); continue; }
						if (strcmp(pAttr->name(), "value") == 0)   prop.value = pAttr->value();
					}
				}
			}
		}
	}

	return !pTileMap->m_objects.empty();
}

std::vector<TileMapManager::TilesetData> TileMapManager::parseTilesets(const rapidxml::xml_node<char>* pMapNode) noexcept
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

std::vector<glm::uint32_t> TileMapManager::parseCSVstring(const rapidxml::xml_node<char>* pDataNode) noexcept
{
	std::string data(pDataNode->value());

	std::size_t amount = std::count_if(data.begin(), data.end(), [](char c){ return c == ','; }) + 1;
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<glm::uint32_t> parsed_layer;
	parsed_layer.reserve(amount);

	std::stringstream sstream(data);
	{
		glm::uint32_t tile_num = 0;

		while (sstream >> tile_num)
			parsed_layer.push_back(tile_num);
	}

	return parsed_layer;
}

void TileMapManager::unloadOnGPU(const std::unordered_map<TileMap::TilePlane::TileLayer*, std::vector<Vertex2D>>& vertexMap) noexcept
{
	for(const auto& [layer, vertices] : vertexMap)
	{
		glGenVertexArrays(1, &layer->vao);
		glGenBuffers(1, &layer->vbo);

		glBindVertexArray(layer->vao);

		glBindBuffer(GL_ARRAY_BUFFER, layer->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}