#include "Loaders/Assetloader.hpp"

#include "Utils/Files.hpp"

AssetLoader* AssetLoader::m_instance;

AssetLoader::AssetLoader() noexcept
{
    if(!m_instance)
        m_instance = this;
}

AssetLoader::~AssetLoader()
{
}

Texture2D* AssetLoader::getTexture(const std::string& filename) noexcept
{
    if(!m_instance)
        return nullptr;

    if (auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
        return &it->second;

    const auto filepath = FileUtils::getPathToFile(filename, "textures");

    if(filepath.empty())
        return nullptr;

    auto& texture = m_instance->m_textures[filename];
    
    if( ! texture.loadFromFile(filepath))
    {
        m_instance->m_textures.erase(filename);
        return nullptr;
    }

    return &texture;
}