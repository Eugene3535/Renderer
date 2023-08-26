#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader
{
public:
    Shader() noexcept;
    
    bool compile(const std::string& filename, unsigned type) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept;

    int getUniformLocation(const char* name) const noexcept;

    static void bind(const Shader* shader) noexcept;

private:
    std::string readShaderSourceFromFile(const std::string& filename);
    unsigned    compileShaderFromSource(const std::string& source, unsigned type);
    void        linkShaderToProgram(unsigned shader);

private:
    unsigned m_program;

private:
    friend class AssetManager;
};
#endif