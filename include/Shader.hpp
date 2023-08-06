#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

#include "Color.hpp"

class Shader
{
public:
    Shader() noexcept;
    ~Shader();
    
    bool compile(const std::string& filename, GLenum type) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept;

    void bind() noexcept;
    void unbind() noexcept;

    void setUniform(const char* name, bool value) noexcept;
    void setUniform(const char* name, int value) noexcept;
    void setUniform(const char* name, float value) noexcept;
    void setUniform(const char* name, float x, float y) noexcept;
    void setUniform(const char* name, float x, float y, float z) noexcept;
    void setUniform(const char* name, float x, float y, float z, float w) noexcept;
    void setUniform(const char* name, const glm::vec2& value) noexcept;
    void setUniform(const char* name, const glm::vec3& value) noexcept;
    void setUniform(const char* name, const glm::vec4& value) noexcept;
    void setUniform(const char* name, const glm::mat4& matrix) noexcept;
    void setUniform(const char* name, const Color& color) noexcept;

private:
    void checkCompileErrors(GLuint shader, std::string type);

    GLint getUniformLoc(const char* name);

public:
    std::unordered_map<const char*, GLint> m_uniformLocs;
    GLuint                                 m_handle;
};
#endif