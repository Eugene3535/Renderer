#version 460 core

out vec4 FragColor;

in vec2 tex_coord;
in vec4 color;

uniform sampler2D texture0;
//uniform sampler2D texture1;

void main()
{
//    FragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2);
    FragColor = texture(texture0, tex_coord) * color;
}