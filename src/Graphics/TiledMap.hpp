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

		std::uint32_t texture = 0u; // Texture handle
		std::uint32_t count   = 0u; // Number of indices to render
		std::uint32_t vao     = 0u; // Vertex array object
		std::uint32_t vbo     = 0u; // Vertex buffer object
		std::uint32_t ebo     = 0u; // Element buffer object
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