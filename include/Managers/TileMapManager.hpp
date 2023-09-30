#ifndef TILE_MAP_MANAGER_HPP
#define TILE_MAP_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>

#include "rapidxml.hpp"

#include "managers/base/Manager.hpp"
#include "graphics/2D/Vertex2D.hpp"
#include "graphics/2D/TileMap.hpp"

class TileMapManager:
	public Manager
{
private:
	struct TilesetData
	{
		struct Texture2D* pTexture  = nullptr;
		glm::uint32_t     columns   = 0u;
		glm::uint32_t     rows      = 0u;
		glm::uint32_t     tileCount = 0u;
		glm::uint32_t     firstGID  = 1u;
	};

public:
	TileMapManager() noexcept;
	~TileMapManager();

	const TileMap* loadFromFile(const std::string& filename) noexcept;
	const TileMap* get(const std::string& filename) noexcept;
	void clear() noexcept;
	
private:
	bool loadTilePlanes(const rapidxml::xml_node<char>* pMapNode) noexcept;
	bool loadObjects(const rapidxml::xml_node<char>* pMapNode)    noexcept;

private:
	std::vector<TilesetData>   parseTilesets(const rapidxml::xml_node<char>* pMapNode)  noexcept;
	std::vector<glm::uint32_t> parseCSVstring(const rapidxml::xml_node<char>* pMapNode) noexcept;

	void unloadOnGPU(const std::vector<Vertex2D>& vertices, const std::vector<glm::uint32_t>& indices) noexcept;

private:
	std::vector<std::unique_ptr<TileMap>> m_tileMaps;
};

#endif
