#ifndef VERTEX2D_HPP
#define VERTEX2D_HPP

#include "Graphics/Color.hpp"

struct Vertex2D
{
	Vertex2D(): 
		x(0.0f), y(0.0f), u(0.0f), v(0.0f), color(Color::White)
	{
	}

	Vertex2D(float x0, float y0, float u0, float v0, const Color& clr = Color::White): 
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