#include <glad/glad.h>

#include <sstream>
#include <iostream>
#include <algorithm>

#include "rapidxml_utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "system/FileProvider.hpp"
#include "managers/AssetManager.hpp"
#include "graphics/TiledMap.hpp"
#include "managers/TiledMapManager.hpp"

TiledMapManager::TiledMapManager() noexcept
{
}

TiledMapManager::~TiledMapManager()
{
}

const TiledMap* TiledMapManager::loadFromFile(const std::string& filename) noexcept
{
	const auto loaded = get(filename);
	
	if(loaded)
		return loaded;

	const std::string filepath = FileProvider::getPathToFile(filename);

	if(filepath.empty())
		return nullptr;

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	document->parse<0>(xmlFile.data());
	const rapidxml::xml_node<char>* mapNode = document->first_node("map");

	if( ! mapNode )
		return nullptr;

	auto& tiledMap = m_tiledMaps.emplace_back(std::make_unique<TiledMap>());
	tiledMap->m_name = filename;

	if ( loadTileLayers(mapNode) && loadObjects(mapNode) )
		return tiledMap.get();

	m_tiledMaps.pop_back();

	return nullptr;
}

const TiledMap* TiledMapManager::get(const std::string& filename) noexcept
{
	for(const auto& tilemap : m_tiledMaps)
		if(tilemap->m_name == filename)
			return tilemap.get();

    return nullptr;
}

void TiledMapManager::draw(const TiledMap::Layer& layer) const noexcept
{
	glBindTexture(GL_TEXTURE_2D, layer.texture);
	glBindVertexArray(layer.vao);

	glDrawElements(GL_TRIANGLES, layer.count, GL_UNSIGNED_INT, nullptr);
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TiledMapManager::clear() noexcept
{
	m_tiledMaps.clear();
}

bool TiledMapManager::loadTileLayers(const rapidxml::xml_node<char>* mapNode) noexcept
{
	std::vector<TilesetData> tilesets = parseTilesets(mapNode);

	if (tilesets.empty())
		return false;

	auto mapW  = mapNode->first_attribute("width");
	auto mapH  = mapNode->first_attribute("height");
	auto tileW = mapNode->first_attribute("tilewidth");
	auto tileH = mapNode->first_attribute("tileheight");

	const std::int32_t map_width   = mapW  ? std::atoi(mapW->value())  : 0u;
	const std::int32_t map_height  = mapH  ? std::atoi(mapH->value())  : 0;
	const std::int32_t tile_width  = tileW ? std::atoi(tileW->value()) : 0;
	const std::int32_t tile_height = tileH ? std::atoi(tileH->value()) : 0;

	auto tiledMap = m_tiledMaps.back().get();

	tiledMap->m_mapSize  = { map_width, map_height };
	tiledMap->m_tileSize = { tile_width, tile_height };

	for (auto layerNode = mapNode->first_node("layer");
			  layerNode != nullptr;
			  layerNode = layerNode->next_sibling("layer"))
	{
		auto pName = layerNode->first_attribute("name");
		std::string name = pName ? pName->value() : std::string();

		if (name.empty())
			continue;

		auto dataNode = layerNode->first_node("data");

		if (!dataNode)
			continue;

		std::vector<std::int32_t> parsed_layer = parseCSVstring(dataNode);

		std::size_t non_zero_tile_count = std::count_if(parsed_layer.begin(), parsed_layer.end(),
			[](std::int32_t n) { return n > 0; });

		const auto bounds = std::minmax_element(parsed_layer.begin(), parsed_layer.end());
		std::int32_t minTile = *bounds.first;
		std::int32_t maxTile = *bounds.second;

		auto currentTileset = std::find_if(tilesets.begin(), tilesets.end(),
		[minTile, maxTile](const TilesetData& ts)
		{
			return minTile <= ts.firstGID && maxTile <= ts.firstGID + ts.tileCount;
		});

		if(currentTileset == tilesets.end())
			continue;

		auto& layer = tiledMap->m_layers.emplace_back();
		layer.name = name;
		layer.texture = currentTileset->texture->getNativeHandle();

		std::vector<Vertex2D> vertices;
		std::vector<glm::uint32_t> indices;

		vertices.reserve(non_zero_tile_count * 4);
		indices.reserve(non_zero_tile_count * 6);

		auto ratio = 1.0f / glm::vec2(currentTileset->texture->getSize());

		for (std::int32_t y = 0u; y < map_height; ++y)
			for (std::int32_t x = 0u; x < map_width; ++x)
			{
				std::int32_t tile_id = parsed_layer[y * map_width + x];

				if (tile_id)
				{
					std::int32_t tile_num = tile_id - currentTileset->firstGID;

					std::int32_t Y = (tile_num >= currentTileset->columns) ? tile_num / currentTileset->columns : 0u;
					std::int32_t X = tile_num % currentTileset->columns;

					std::int32_t offsetX = X * tile_width;
					std::int32_t offsetY = Y * tile_height;

					float left   = offsetX * ratio.x;
					float top    = offsetY * ratio.y;
					float right  = (offsetX + tile_width) * ratio.x;
					float bottom = (offsetY + tile_height) * ratio.y;

					glm::vec2 leftBottom  = { x * tile_width,              y * tile_height + tile_height };
					glm::vec2 rightBootom = { x * tile_width + tile_width, y * tile_height + tile_height };
					glm::vec2 rightTop    = { x * tile_width + tile_width, y * tile_height };
					glm::vec2 leftTop     = { x * tile_width,              y * tile_height };

					glm::uint32_t index = static_cast<glm::uint32_t>(vertices.size());

					vertices.emplace_back(leftBottom.x, leftBottom.y, left, bottom);
					vertices.emplace_back(rightBootom.x, rightBootom.y, right, bottom);
					vertices.emplace_back(rightTop.x, rightTop.y, right, top);
					vertices.emplace_back(leftTop.x, leftTop.y, left, top);

					indices.push_back(index);
					indices.push_back(index + 1u);
					indices.push_back(index + 2u);

					indices.push_back(index);
					indices.push_back(index + 2u);
					indices.push_back(index + 3u);
				}
			}
		unloadOnGPU(vertices, indices);
	}

	return true;
}

bool TiledMapManager::loadObjects(const rapidxml::xml_node<char>* mapNode) noexcept
{
	auto tiledMap = m_tiledMaps.back().get();

	for (auto objectGroupNode = mapNode->first_node("objectgroup");
			  objectGroupNode != nullptr;
			  objectGroupNode = objectGroupNode->next_sibling("objectgroup"))
	{
		for (auto objectNode = objectGroupNode->first_node("object");
				  objectNode != nullptr;
				  objectNode = objectNode->next_sibling("object"))
		{
			auto& tme_object = tiledMap->m_objects.emplace_back();

			for (auto attr = objectNode->first_attribute(); attr != nullptr; attr = attr->next_attribute())
			{
				if (strcmp(attr->name(), "x")      == 0) { tme_object.position.x = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "y")      == 0) { tme_object.position.y = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "width")  == 0) { tme_object.size.x     = std::atoi(attr->value()); continue; }
				if (strcmp(attr->name(), "height") == 0) { tme_object.size.y     = std::atoi(attr->value()); continue; }

				if (strcmp(attr->name(), "name")  == 0) tme_object.name = attr->value();
				if (strcmp(attr->name(), "class") == 0) tme_object.type = attr->value();
			}

			if (const auto propertiesNode = objectNode->first_node("properties"); propertiesNode != nullptr)
			{
				for (auto propertyNode = propertiesNode->first_node("property");
					      propertyNode != nullptr;
					      propertyNode = propertyNode->next_sibling("property"))
				{
					auto& prop = tme_object.properties.emplace_back();

					for (auto attr = propertyNode->first_attribute(); attr != nullptr; attr = attr->next_attribute())
					{
						if (strcmp(attr->name(), "name")  == 0) { prop.name  = attr->value(); continue; }
						if (strcmp(attr->name(), "type")  == 0) { prop.type  = attr->value(); continue; }
						if (strcmp(attr->name(), "value") == 0)   prop.value = attr->value();
					}
				}
			}
		}
	}

	return !tiledMap->m_objects.empty();
}

std::vector<TiledMapManager::TilesetData> TiledMapManager::parseTilesets(const rapidxml::xml_node<char>* mapNode) noexcept
{
	std::vector<TilesetData> tilesets;

	for (auto tilesetNode = mapNode->first_node("tileset");
		      tilesetNode != nullptr;
		      tilesetNode = tilesetNode->next_sibling("tileset"))
	{
		auto imageNode = tilesetNode->first_node("image");
		auto sourceNode = imageNode->first_attribute("source");

		std::string texName = sourceNode ? sourceNode->value() : std::string();

		if (texName.empty())
			continue;

		std::size_t last_slash_pos = texName.find_last_of('/');

		if (last_slash_pos != std::string::npos)	
			texName.erase(0, last_slash_pos + 1);
		
		auto tileset = AssetManager::get<Texture2D>(texName);

		if( tileset == nullptr )
			continue;

		TilesetData& ts = tilesets.emplace_back();

		auto tileCount = tilesetNode->first_attribute("tilecount");
		auto columns   = tilesetNode->first_attribute("columns");
		auto firstGID  = tilesetNode->first_attribute("firstgid");

		ts.texture  = tileset;
		ts.tileCount = tileCount ? std::atoi(tileCount->value()) : 0;
		ts.columns   = columns ? std::atoi(columns->value()) : 0;
		ts.rows      = ( ! tileCount || ! columns ) ? 0 : ts.tileCount / ts.columns;
		ts.firstGID  = firstGID ? std::atoi(firstGID->value()) : 0;
	}

	return tilesets;
}

std::vector<std::int32_t> TiledMapManager::parseCSVstring(const rapidxml::xml_node<char>* dataNode) noexcept
{
	std::string data(dataNode->value());

	std::size_t amount = std::count_if(data.begin(), data.end(), [](char c){ return c == ','; }) + 1;
	std::replace(data.begin(), data.end(), ',', ' ');

	std::vector<std::int32_t> parsed_layer;
	parsed_layer.reserve(amount);

	std::stringstream sstream(data);
	{
		std::int32_t tile_num = 0;

		while (sstream >> tile_num)
			parsed_layer.push_back(tile_num);
	}

	return parsed_layer;
}

void TiledMapManager::unloadOnGPU(const std::vector<Vertex2D>& vertices, const std::vector<glm::uint32_t>& indices) noexcept
{
	if(m_tiledMaps.empty())
		return;

	auto& layer = m_tiledMaps.back()->m_layers.back();
	layer.count = indices.size();

	glGenVertexArrays(1, &layer.vao);
	glGenBuffers(1, &layer.vbo);
	glGenBuffers(1, &layer.ebo);

	glBindVertexArray(layer.vao);

	glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layer.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}