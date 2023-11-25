#include "managers/AssetManager.hpp"

AssetManager* AssetManager::m_instance;

AssetManager::AssetManager() noexcept
{
    if(!m_instance)
        m_instance = this;
}