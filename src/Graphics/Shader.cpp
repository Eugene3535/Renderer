#include "Graphics/Shader.hpp"
#include "Utils/Files.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() noexcept: 
    m_handle(0)
{
    m_handle = glCreateProgram();

    setUniform1i = glUniform1i;
    setUniform1f = glUniform1f;
    setUniform2f = glUniform2f;
    setUniform3f = glUniform3f;
    setUniform4f = glUniform4f;
}

Shader::~Shader()
{
    glDeleteProgram(m_handle);
}

bool Shader::compile(const std::string &filename, unsigned type) noexcept
{
    const std::string filepath = FileUtils::getPathToFile(filename);

    if (filepath.empty())
        return false;

    std::string source;
    std::ifstream ifs_stream;

    ifs_stream.open(filepath);

    if (ifs_stream.is_open())
    {
        std::stringstream s_stream;
        s_stream << ifs_stream.rdbuf();
        ifs_stream.close();

        source = s_stream.str();
    }
    else
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        return false;
    }

    GLuint shader = glCreateShader(type);

    const GLchar* c_str = source.c_str();

    glShaderSource(shader, 1, &c_str, 0);
    glCompileShader(shader);
    checkCompileErrors(shader, "SHADER");

    glAttachShader(m_handle, shader);
    glLinkProgram(m_handle);
    checkCompileErrors(m_handle, "PROGRAM");

    glDeleteShader(shader);

    return true;
}

bool Shader::compile(const std::string &filename_vert, const std::string &filename_frag) noexcept
{
    return compile(filename_vert, GL_VERTEX_SHADER) && compile(filename_frag, GL_FRAGMENT_SHADER);
}

bool Shader::compile(const std::string &filename_vert, const std::string &filename_frag, const std::string &filename_geom) noexcept
{
    return compile(filename_vert, GL_VERTEX_SHADER) && compile(filename_frag, GL_FRAGMENT_SHADER) && compile(filename_geom, GL_GEOMETRY_SHADER);
}

void Shader::bind() noexcept
{
    glUseProgram(m_handle);
}

void Shader::unbind() noexcept
{
    glUseProgram(0);
}

int Shader::getUniformLocation(const char *name) const noexcept
{
    return glGetUniformLocation(m_handle, name);
}

GLuint Shader::getHandle() const noexcept
{
    return m_handle;
}

void Shader::setUniform(int loc, const glm::vec2& value) noexcept
{
    if (loc != -1)
        glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Shader::setUniform(int loc, const glm::vec3& value) noexcept
{
    if (loc != -1)
        glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setUniform(int loc, const glm::vec4& value) noexcept
{
    if (loc != -1)
        glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Shader::setUniform(int loc, const glm::mat4& matrix) noexcept
{
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(int loc, const float* matrix) noexcept
{
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    int success;
    char infoLog[1024]{};

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
}