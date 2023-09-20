#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 ModelViewProjection = mat4(1.0f);

out vec2 tex_coord;

void main()
{
    gl_Position = ModelViewProjection * vec4(position.x, position.y, 0.0f, 1.0f);
    
    tex_coord = texCoords;
}
