#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "Graphics/Texture2D.hpp"

class AssetLoader
{
public:
    AssetLoader() noexcept;
    ~AssetLoader();

    static class Texture2D* getTexture(const std::string& filename) noexcept;

private:
    std::unordered_map<std::string, Texture2D> m_textures;

private:
    static AssetLoader* m_instance;
};




#endif