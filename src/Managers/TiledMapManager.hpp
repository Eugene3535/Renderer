#ifndef TILED_MAP_MANAGER_HPP
#define TILED_MAP_MANAGER_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "rapidxml.hpp"

#include "system/NonCopyable.hpp"
#include "graphics/Vertex2D.hpp"

class TiledMapManager:
	private NonCopyable
{
private:
	struct TilesetData
	{
		class Texture2D* texture  = nullptr;
		int columns   = 0;
		int rows      = 0;
		int tileCount = 0;
		int firstGID  = 1;
	};

public:
	TiledMapManager() noexcept;
	~TiledMapManager();

	const struct TiledMap* loadFromFile(const std::string& filename) noexcept;
	const struct TiledMap* get(const std::string& filename) noexcept;
	void draw(const TiledMap::Layer& layer) const noexcept;
	void clear() noexcept;
	
private:
	bool loadTileLayers(const rapidxml::xml_node<char>* mapNode) noexcept;
	bool loadObjects(const rapidxml::xml_node<char>* mapNode)    noexcept;

private:
	std::vector<TilesetData>  parseTilesets(const rapidxml::xml_node<char>* mapNode)   noexcept;
	std::vector<int> parseCSVstring(const rapidxml::xml_node<char>* dataNode) noexcept;

	void unloadOnGPU(const std::vector<Vertex2D>& vertices, const std::vector<std::uint32_t>& indices) noexcept;

private:
	std::vector<std::unique_ptr<TiledMap>> m_tiledMaps;
};

#endif // !TILED_MAP_MANAGER_HPP
