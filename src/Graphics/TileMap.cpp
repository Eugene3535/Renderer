#include <glad/glad.h>

#include "Graphics/Texture2D.hpp"
#include "Graphics/TileMap.hpp"

TileMap::TileMap() noexcept
{

}

TileMap::~TileMap()
{
	for (auto& plane : m_tilePlanes)
		for (auto& layer : plane.tileLayers)
		{
			if(layer.vao)
				glDeleteVertexArrays(1, &layer.vao);

			if(layer.vbo)
				glDeleteBuffers(1, &layer.vbo);
		}		
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

	for(auto& obj : m_objects)
		if(obj.name == name)
			objects.push_back(&obj);

	return objects;
}

std::vector<const TileMap::Object*> TileMap::getObjectsByType(const std::string& type) const noexcept
{
	std::vector<const TileMap::Object*> objects;

	for(auto& obj : m_objects)
		if(obj.type == type)
			objects.push_back(&obj);

	return objects;
}

const glm::uvec2& TileMap::getMapSizeInTiles()  const noexcept
{
	return m_mapSize;
}

const glm::uvec2& TileMap::getTileSize() const noexcept
{
	return m_tileSize;
}

glm::uvec2 TileMap::getMapSizeInPixels() const noexcept
{
	return { m_mapSize.x * m_tileSize.x, m_mapSize.y * m_tileSize.y };
}

void TileMap::draw() noexcept
{
	for (auto& plane : m_tilePlanes)
		for (auto& layer : plane.tileLayers)
		{
			glBindTexture(GL_TEXTURE_2D, layer.pTexture->texture);
			glBindVertexArray(layer.vao);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned>(layer.indices.size()), GL_UNSIGNED_INT, layer.indices.data());
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
}