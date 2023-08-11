#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <type_traits>

#include <glad/glad.h>

#include "Utils/Files.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Shader.hpp"

class AssetManager
{
public:
    AssetManager() noexcept;
    ~AssetManager() = default;

    template <class T, class... Args>
    static T* get(const std::string& filename, Args&& ...args) noexcept
    {
        if( ! m_pInstance )
            return nullptr;

//      Textures
        if constexpr (std::is_same<T, Texture2D>::value) 
        {
            if(auto it = m_pInstance->m_textures.find(filename); it != m_pInstance->m_textures.end())	
                return &it->second;

            return tryLoadFromFile<Texture2D>(filename, "textures", m_pInstance->m_textures);
        }
        //      Shaders    
        else if constexpr (std::is_same<T, sf::Shader>::value)
        {
            if(auto it = m_pInstance->m_shaders.find(filename); it != m_pInstance->m_shaders.end())	
                return &it->second;

            return tryLoadFromFile<Shader>(filename, "shaders", m_pInstance->m_shaders, std::forward<Args>(args)...);
        }

        return nullptr;
    }

    template<class T>
    static void remove(const std::string& filename) noexcept
    {
        if( ! m_pInstance )
            return;

//      Textures
        if constexpr (std::is_same<T, sf::Texture>::value) 
        {
            if(auto it = m_pInstance->m_textures.find(filename); it != m_pInstance->m_textures.end())	
                m_pInstance->m_textures.erase(it);
        }
        //      Shaders
        else if constexpr (std::is_same<T, sf::Shader>::value) 
        {
            if(auto it = m_pInstance->m_shaders.find(filename); it != m_pInstance->m_shaders.end())	
                m_pInstance->m_shaders.erase(it);
        }
    }

    static void clear() noexcept
    {
        if( ! m_pInstance )
            return;

        m_pInstance->m_textures.clear();
    }

private:
    template<class T, class... Args>
    static T* tryLoadFromFile(const std::string& filename, const std::string& folder, std::unordered_map<std::string, T>& container, Args&& ...args) noexcept 
    {
        const std::string filepath = FileUtils::getPathToFile(filename, folder);

        if( ! filepath.empty() )
        {
            auto [iterator, result] = container.try_emplace(filename);

            if(result)
            {
//              Shader case with variadic templates
                if constexpr (std::is_same<T, Shader>::value)
                {
                    if( ! iterator->second.compile(filepath, std::forward<Args>(args)...))
                    {
                        container.erase(filename);

                        return nullptr;	
                    }
                }
//              Image, Texture, etc...                     
                else if( ! iterator->second.loadFromFile(filepath))
                {
                    container.erase(filename);

                    return nullptr;	
                }
            }
            return &iterator->second;
        }

        return nullptr;	
    }

private:
    std::unordered_map<std::string, Texture2D> m_textures;
    std::unordered_map<std::string, Shader> m_shaders;

private:
    static AssetManager* m_pInstance;
};




#endif