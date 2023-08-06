#include "Loaders/Assetloader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

#include "stb_image.h"

#include "Utils/Files.hpp"
#include "Utils/Structures.hpp"

AssetLoader* AssetLoader::m_instance;

AssetLoader::AssetLoader() noexcept
{
    if(!m_instance)
        m_instance = this;
}

AssetLoader::~AssetLoader()
{
    for(const auto& [name, texture] : m_instance->m_textures)
        glDeleteTextures(1, &texture.id);
}

const Texture2D* AssetLoader::getTexture(const std::string& filename) noexcept
{
    if(!m_instance)
        return nullptr;

    if (auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
        return &it->second;

    const auto filepath = FileUtils::getPathToFile(filename, "textures");

    if(filepath.empty())
        return nullptr;

    Texture2D texture;
    glGenTextures(1, &texture.id);

    int bytePerPixel = 0;
    unsigned char* data = stbi_load(filepath.c_str(), &texture.width, &texture.height, &bytePerPixel, STBI_rgb_alpha);

    if ( ! data)
        return nullptr;

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);   

    stbi_image_free(data); 

    auto it = m_instance->m_textures.emplace(filename, texture);

    return &it.first->second;
}