#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/glad.h>

#include "system/NonCopyable.hpp"

class Shader:
    private NonCopyable
{
public:
    Shader() noexcept;
    ~Shader();
    
    bool compile(const std::string& filename, GLuint type) noexcept;
    bool compile(const std::string& filenameVert, const std::string& filenameFrag) noexcept;
    bool compile(const std::string& filenameVert, const std::string& filenameFrag, const std::string& filenameGeom) noexcept;

    GLint getUniformLocation(const char* name) const noexcept;

    static void bind(const Shader* shader) noexcept;

private:
    std::string readShaderSourceFromFile(const std::string& filename);
    GLuint      compileShaderFromSource(const std::string& source, GLuint type);
    bool        linkToProgram(GLuint shader);

private:
    GLuint m_program;
};
#endif