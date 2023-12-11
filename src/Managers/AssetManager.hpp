#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <type_traits>

#include <glad/glad.h>

#include "system/NonCopyable.hpp"
#include "system/FileProvider.hpp"
#include "graphics/Image.hpp"
#include "graphics/Texture2D.hpp"
#include "graphics/Shader.hpp"

class AssetManager:
	private NonCopyable
{
public:
    AssetManager() noexcept;
    ~AssetManager() = default;

    template <class T, class... Args>
    static T* get(const std::string& filename, Args&& ...args) noexcept
    {
        if(m_instance)
        {
//          Textures
            if constexpr (std::is_same<T, Texture2D>::value)
            {
                if(auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
                    return &it->second;

                return tryLoadFromFile<Texture2D>(filename, m_instance->m_textures);
            }
//          Shaders
            else if constexpr (std::is_same<T, Shader>::value)
            {
                if(auto it = m_instance->m_shaders.find(filename); it != m_instance->m_shaders.end())
                    return &it->second;

                return tryLoadFromFile<Shader>(filename, m_instance->m_shaders, std::forward<Args>(args)...);
            }
        }

        return nullptr;
    }

    template<class T>
    static void remove(const std::string& filename) noexcept
    {
        if( ! m_instance )
            return;

//      Textures
        if constexpr (std::is_same<T, Texture2D>::value)
        {
            if(auto it = m_instance->m_textures.find(filename); it != m_instance->m_textures.end())
                m_instance->m_textures.erase(it);
        }
//      Shaders
        else if constexpr (std::is_same<T, Shader>::value)
        {
            if(auto it = m_instance->m_shaders.find(filename); it != m_instance->m_shaders.end())
                m_instance->m_shaders.erase(it);
        }
    }

    static void clear() noexcept
    {
        if( ! m_instance )
            return;

        m_instance->m_textures.clear();
    }

private:
    template<class T, class... Args>
    static T* tryLoadFromFile(const std::string& filename, std::unordered_map<std::string, T>& container, Args&& ...args) noexcept
    {
        if constexpr (std::is_same<T, Shader>::value)
        {
            auto [iterator, result] = container.try_emplace(filename);

            if (result)
            {
                auto shader = &iterator->second;

                if (!shader->compile(std::forward<Args>(args)...))          
                    container.erase(filename);
                else
                    return shader;
            }
        }
        else // Image, Texture2D...
        {
            const std::string filepath = FileProvider::getPathToFile(filename);

            if (!filepath.empty())
            {
                auto [iterator, result] = container.try_emplace(filename);

                if (result)
                {
                    auto& texture = iterator->second;

                    if(!texture.loadFromFile(filepath))
                        container.erase(filename);                
                    else           
                        return &texture;
                }
            }
        }

        return nullptr;
    }

private:
    std::unordered_map<std::string, Texture2D> m_textures;
    std::unordered_map<std::string, Shader>    m_shaders;

private:
    static AssetManager* m_instance;
};

#endif
