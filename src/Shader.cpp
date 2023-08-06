#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() noexcept : 
    m_handle(0)
{
    m_handle = glCreateProgram();
}

Shader::~Shader()
{
    if (m_handle)
        glDeleteProgram(m_handle);
}

bool Shader::compile(const std::string &filename, GLenum type) noexcept
{
    std::string source;
    std::ifstream ifs_stream;

    ifs_stream.open(filename);

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

    const GLchar *c_str = source.c_str();

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


void Shader::setUniform(const char *name, bool value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform1i(loc, (int)value);
}

void Shader::setUniform(const char *name, int value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform1i(loc, value);
}

void Shader::setUniform(const char *name, float value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform1f(loc, value);
}

void Shader::setUniform(const char *name, float x, float y) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform2f(loc, x, y);
}

void Shader::setUniform(const char *name, float x, float y, float z) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform3f(loc, x, y, z);
}

void Shader::setUniform(const char *name, float x, float y, float z, float w) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform4f(loc, x, y, z, w);
}

void Shader::setUniform(const char *name, const glm::vec2 &value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform2fv(loc, 1, &value[0]);
}

void Shader::setUniform(const char *name, const glm::vec3 &value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform3fv(loc, 1, &value[0]);
}

void Shader::setUniform(const char *name, const glm::vec4 &value) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniform4fv(loc, 1, &value[0]);
}

void Shader::setUniform(const char *name, const glm::mat4 &matrix) noexcept
{
    GLint loc = getUniformLoc(name);

    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const char *name, const Color& color) noexcept
{   
    setUniform(name, glm::vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024]{};

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

GLint Shader::getUniformLoc(const char *name)
{
    auto found = m_uniformLocs.find(name);

    if (found == m_uniformLocs.end())
    {
        GLint loc = glGetUniformLocation(m_handle, name);

        if (loc == -1)
            return -1;

        bool sucsess = m_uniformLocs.try_emplace(name, loc).second;

        return sucsess ? loc : -1;
    }

    return found->second;
}