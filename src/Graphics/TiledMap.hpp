#ifndef TILEDMAP_HPP
#define TILEDMAP_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct TiledMap
{
	struct Layer
	{
		std::string name;

		unsigned texture = 0U; // Texture handle
		unsigned count   = 0U; // Number of indices to render
		unsigned vao     = 0U; // Vertex array object
		unsigned vbo     = 0U; // Vertex buffer object
		unsigned ebo     = 0U; // Element buffer object
	};

	struct Object
	{
		struct Property
		{
			std::string name;
			std::string type;
			std::string value;
		};

		std::string name;
		std::string type;

		std::vector<Property> properties;

		glm::ivec2 position;
		glm::ivec2 size;
	};

    std::vector<Layer>  m_layers;
    std::vector<Object> m_objects;
    std::string         m_name;
    glm::uvec2          m_mapSize;
    glm::uvec2          m_tileSize;
};

#endif // !TILED_MAP_HPP