#include "Managers/AssetManager.hpp"

AssetManager* AssetManager::m_pInstance;

AssetManager::AssetManager() noexcept
{
    if(!m_pInstance)
        m_pInstance = this;
}