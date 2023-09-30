#include <glad/glad.h>

#include "graphics/2D/Texture2D.hpp"
#include "graphics/2D/TileMap.hpp"

TileMap::TileMap() noexcept
{

}

TileMap::~TileMap()
{
	for (auto& layer : m_tileLayers)
	{
		if(layer.vao)
			glDeleteVertexArrays(1, &layer.vao);

		if(layer.vbo)
			glDeleteBuffers(1, &layer.vbo);

		if (layer.ebo)
			glDeleteBuffers(1, &layer.ebo);
	}		
}

const std::vector<TileMap::TileLayer>& TileMap::getTileLayers() const noexcept
{
	return m_tileLayers;
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

void TileMap::draw(const TileLayer& layer) const noexcept
{
	glBindVertexArray(layer.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layer.ebo);

	glBindTexture(GL_TEXTURE_2D, layer.texture);
	glDrawElements(GL_TRIANGLES, layer.count, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}