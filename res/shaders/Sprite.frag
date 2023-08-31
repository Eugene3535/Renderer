#version 460 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, tex_coord);
}