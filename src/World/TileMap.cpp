#include "World/TileMap.hpp"

#include <glad/glad.h>

#include <sstream>
#include <iostream>
#include <algorithm>

#include "rapidxml_utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Files.hpp"
#include "Utils/Defines.hpp"
#include "Geometry/Vector2.hpp"
#include "Managers/AssetManager.hpp"

TileMap::TileMap() noexcept:
	m_mapSize(),
	m_tileSize()
{
}

TileMap::~TileMap()
{
	for (auto& plane : m_tilePlanes)
		for (auto& layer : plane.m_tileLayers)
		{
			if(layer.vao)
				glDeleteVertexArrays(1, &layer.vao);

			if(layer.vbo)
				glDeleteBuffers(1, &layer.vbo);
		}		
}

bool TileMap::loadFromFile(const std::string& filename) noexcept
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

	return (loadTilePlanes(pMapNode) && loadObjects(pMapNode));
}

const std::vector<TileMap::TilePlane>& TileMap::getTilePlanes() const noexcept
{
	return m_tilePlanes;
}

const std::vector<TileMap::Object>& TileMap::getObjects() const noexcept
{
	return m_objects;
}

std::vector<const TileMap::Object*> TileMap::getObjectsByName(const std::string& name) const noexcept
{
	std::vector<const TileMap::Object*> objects;

	for (auto& obj : m_objects)
		if (obj.name == name)
			objects.push_back(std::addressof(obj));

	return objects;
}

std::vector<const TileMap::Object*> TileMap::getObjectsByType(const std::string& type) const noexcept
{
	std::vector<const TileMap::Object*> objects;

	for (auto& obj : m_objects)
		if (obj.type == type)
			objects.push_back(std::addressof(obj));

	return objects;
}

const Vector2u& TileMap::getMapSizeInTiles()  const noexcept
{
	return m_mapSize;
}

Vector2u TileMap::getMapSizeInPixels() const noexcept
{
	return { m_mapSize.x * m_tileSize.x, m_mapSize.y * m_tileSize.y };
}

const Vector2u& TileMap::getTileSize() const noexcept
{
	return m_tileSize;
}

void TileMap::draw() noexcept
{
	for (auto& plane : m_tilePlanes)
		for (auto& layer : plane.m_tileLayers)
		{
			glBindTexture(GL_TEXTURE_2D, layer.pTexture->texture);
			glBindVertexArray(layer.vao);
			glDrawElements(GL_TRIANGLES, static_cast<std::uint32_t>(layer.indices.size()), GL_UNSIGNED_INT, layer.indices.data());
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
}

bool TileMap::loadTilePlanes(const rapidxml::xml_node<char>* pMapNode) noexcept
{
	std::vector<TilesetData> tilesets = parseTilesets(pMapNode);

	if (tilesets.empty())
		return false;

	auto pMapW  = pMapNode->first_attribute("width");
	auto pMapH  = pMapNode->first_attribute("height");
	auto pTileW = pMapNode->first_attribute("tilewidth");
	auto pTileH = pMapNode->first_attribute("tileheight");

	const std::uint32_t map_width   = pMapW  ? std::strtol(pMapW->value(),  NULL, 10) : 0;
	const std::uint32_t map_height  = pMapH  ? std::strtol(pMapH->value(),  NULL, 10) : 0;
	const std::uint32_t tile_width  = pTileW ? std::strtol(pTileW->value(), NULL, 10) : 0;
	const std::uint32_t tile_height = pTileH ? std::strtol(pTileH->value(), NULL, 10) : 0;

#ifdef DEBUG
	if (!map_width || !map_height || !tile_width || !tile_height)
		return false;
#endif

	m_mapSize  = Vector2u(map_width, map_height);
	m_tileSize = Vector2u(tile_width, tile_height);

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

		std::vector<std::uint32_t> parsed_layer = parseCSVstring(pDataNode);

		std::size_t non_zero_tile_count = std::count_if(parsed_layer.begin(), parsed_layer.end(),
			[](std::uint32_t n) { return n > 0; });

		auto& plane = m_tilePlanes.emplace_back();
		plane.m_name = name;

		for (std::uint32_t y = 0; y < map_height; ++y)
			for (std::uint32_t x = 0; x < map_width; ++x)
			{
				std::uint32_t tile_id = parsed_layer[y * map_width + x];

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
					std::uint32_t tile_num = tile_id - current_tileset->firstGID;

					// left-top coords of the tile in texture grid
					std::uint32_t Y = (tile_num >= current_tileset->columns) ? tile_num / current_tileset->columns : 0;
					std::uint32_t X = tile_num % current_tileset->columns;

					std::uint32_t offsetX = X * tile_width;
					std::uint32_t offsetY = Y * tile_height;

					auto ratio = 1.0f / Vector2f(pLayer->pTexture->width, pLayer->pTexture->height);

//                  Texture coords
					float left = offsetX * ratio.x;
					float top = offsetY * ratio.y;
					float right = (offsetX + tile_width) * ratio.x;
					float bottom = (offsetY + tile_height) * ratio.y;

//                  Vertex coords
					Vector2f leftBottom  = { static_cast<float>(x * tile_width),              static_cast<float>(y * tile_height + tile_height) };
					Vector2f rightBootom = { static_cast<float>(x * tile_width + tile_width), static_cast<float>(y * tile_height + tile_height) };
					Vector2f rightTop    = { static_cast<float>(x * tile_width + tile_width), static_cast<float>(y * tile_height) };
					Vector2f leftTop     = { static_cast<float>(x * tile_width),              static_cast<float>(y * tile_height) };

//                  Index stride
					std::uint32_t index = static_cast<std::uint32_t>(pLayer->vertices.size());

					// Quad
					pLayer->vertices.emplace_back(leftBottom.x, leftBottom.y, left, bottom);
					pLayer->vertices.emplace_back(rightBootom.x, rightBootom.y, right, bottom);
					pLayer->vertices.emplace_back(rightTop.x, rightTop.y, right, top);
					pLayer->vertices.emplace_back(leftTop.x, leftTop.y, left, top);

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

	for (auto& plane : m_tilePlanes)
		for (auto& layer : plane.m_tileLayers)
			unloadOnGPU(layer);

	return !m_tilePlanes.empty();
}

bool TileMap::loadObjects(const rapidxml::xml_node<char>* pMapNode) noexcept
{
	std::size_t objectAmount = 0;

	for (auto pObjectGroupNode = pMapNode->first_node("objectgroup");
			  pObjectGroupNode != nullptr;
			  pObjectGroupNode = pObjectGroupNode->next_sibling("objectgroup"))
	{
		for (auto pObjectNode = pObjectGroupNode->first_node("object");
				  pObjectNode != nullptr;
				  pObjectNode = pObjectNode->next_sibling("object"))

			objectAmount++;
	}

	m_objects.reserve(objectAmount);

	for (auto pObjectGroupNode = pMapNode->first_node("objectgroup");
			  pObjectGroupNode != nullptr;
			  pObjectGroupNode = pObjectGroupNode->next_sibling("objectgroup"))
	{
		for (auto pObjectNode = pObjectGroupNode->first_node("object");
				  pObjectNode != nullptr;
				  pObjectNode = pObjectNode->next_sibling("object"))
		{
			auto& tme_object = m_objects.emplace_back();

			for (auto pAttr = pObjectNode->first_attribute(); pAttr != nullptr; pAttr = pAttr->next_attribute())
			{
				if (strcmp(pAttr->name(), "x")      == 0) { tme_object.position.x = std::strtol(pAttr->value(), nullptr, 10); continue; }
				if (strcmp(pAttr->name(), "y")      == 0) { tme_object.position.y = std::strtol(pAttr->value(), nullptr, 10); continue; }
				if (strcmp(pAttr->name(), "width")  == 0) { tme_object.size.x     = std::strtol(pAttr->value(), nullptr, 10); continue; }
				if (strcmp(pAttr->name(), "height") == 0) { tme_object.size.y     = std::strtol(pAttr->value(), nullptr, 10); continue; }

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

	return !m_objects.empty();
}

std::vector<TileMap::TilesetData> TileMap::parseTilesets(const rapidxml::xml_node<char>* pMapNode) noexcept
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

std::vector<std::uint32_t> TileMap::parseCSVstring(const rapidxml::xml_node<char>* pDataNode) noexcept
{
	std::string data(pDataNode->value());

	std::size_t amount = std::count_if(data.begin(), data.end(), [](char c){ return c == ','; }) + 1;
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<std::uint32_t> parsed_layer;
	parsed_layer.reserve(amount);

	std::stringstream sstream(data);
	{
		std::uint32_t tile_num = 0;

		while (sstream >> tile_num)
			parsed_layer.push_back(tile_num);
	}

	return parsed_layer;
}

void TileMap::unloadOnGPU(TileMap::TilePlane::TileLayer& layer) noexcept
{
	glGenVertexArrays(1, &layer.vao);
	glGenBuffers(1, &layer.vbo);

	glBindVertexArray(layer.vao);

	glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * layer.vertices.size(), layer.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	layer.vertices.clear();
}