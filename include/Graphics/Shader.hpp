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
    Shader(const Shader& other) = delete;
	Shader(const Shader&& other) = delete;
	Shader& operator = (const Shader& other) = delete;
	Shader& operator = (const Shader&& other) = delete;
    ~Shader();
    
    bool compile(const std::string& filename, GLenum type) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept;

    void bind() noexcept;
    void unbind() noexcept;

    GLint  getUniformLocation(const char* name) const noexcept;
    GLuint getHandle() const noexcept;

    void (*setUniform1i)(GLint loc, GLint value) = nullptr;
    void (*setUniform1f)(GLint loc, GLfloat value) = nullptr;
    void (*setUniform2f)(GLint loc, GLfloat x, GLfloat y) = nullptr;
    void (*setUniform3f)(GLint loc, GLfloat x, GLfloat y, GLfloat z) = nullptr;
    void (*setUniform4f)(GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = nullptr;

    void setUniform(GLint loc, const glm::vec2& value) noexcept;
    void setUniform(GLint loc, const glm::vec3& value) noexcept;
    void setUniform(GLint loc, const glm::vec4& value) noexcept;
    void setUniform(GLint loc, const glm::mat4& matrix) noexcept;

private:
    void checkCompileErrors(GLuint shader, std::string type);

    GLuint m_handle;
};
#endif