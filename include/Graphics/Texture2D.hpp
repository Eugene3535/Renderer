#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>
#include <vector>

struct Texture2D
{
	unsigned texture = 0;
	int      width   = 0;
	int      height  = 0;
};

bool LoadImageFromFile(const std::string& filepath, std::vector<unsigned char>& pixels, int& width, int& height);
void CreateTextureFromImage(const std::vector<unsigned char>& pixels, Texture2D& texture);
void SetTexture2DRepeated(unsigned texture, bool repeat);
void SetTexture2DSmooth(unsigned texture, bool smooth);

#endif