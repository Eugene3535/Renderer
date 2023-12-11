#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include "system/NonCopyable.hpp"

class Shader:
    private NonCopyable
{
public:
    Shader() noexcept;
    ~Shader();
    
    bool compile(const std::string& filename, unsigned type) noexcept;
    bool compile(const std::string& filenameVert, const std::string& filenameFrag) noexcept;
    bool compile(const std::string& filenameVert, const std::string& filenameFrag, const std::string& filenameGeom) noexcept;

    int getUniformLocation(const char* name) const noexcept;

    static void bind(const Shader* shader) noexcept;

private:
    std::string readShaderSourceFromFile(const std::string& filename);
    unsigned    compileShaderFromSource(const std::string& source, unsigned type);
    bool        linkToProgram(unsigned shader);

private:
    unsigned m_program;
};
#endif