#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Color.hpp"

struct Texture2D
{
	GLuint id = 0;
	int width = 0;
	int height = 0;
};

struct Vertex
{
	Vertex(): 
		x(0.0f), y(0.0f), u(0.0f), v(0.0f), color(Color::White)
	{
	}

	Vertex(float x0, float y0, float u0, float v0, const Color& clr = Color::White): 
		x(x0), y(y0), u(u0), v(v0), color(clr)
	{
	}

    float x;
	float y;
	float u;
	float v;
	Color color;
};

#endif