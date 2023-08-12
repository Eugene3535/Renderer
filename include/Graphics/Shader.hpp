#ifndef SHADER_HPP
#define SHADER_HPP

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
    
    bool compile(const std::string& filename, unsigned type) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag) noexcept;
    bool compile(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept;

    void bind() noexcept;
    void unbind() noexcept;

    int  getUniformLocation(const char* name) const noexcept;
    unsigned getHandle() const noexcept;

    void (*setUniform1i)(int loc, int value) = nullptr;
    void (*setUniform1f)(int loc, float value) = nullptr;
    void (*setUniform2f)(int loc, float x, float y) = nullptr;
    void (*setUniform3f)(int loc, float x, float y, float z) = nullptr;
    void (*setUniform4f)(int loc, float x, float y, float z, float w) = nullptr;

    void setUniform(int loc, const glm::vec2& value) noexcept;
    void setUniform(int loc, const glm::vec3& value) noexcept;
    void setUniform(int loc, const glm::vec4& value) noexcept;
    void setUniform(int loc, const glm::mat4& matrix) noexcept;

private:
    void checkCompileErrors(unsigned shader, std::string type);

    unsigned m_handle;
};
#endif