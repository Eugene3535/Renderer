#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

out vec2 tex_coord;

void main()
{
    gl_Position = projection * view * model * vec4(inPos.x, inPos.y, 0.0f, 1.0f);

    tex_coord = texCoords;
}