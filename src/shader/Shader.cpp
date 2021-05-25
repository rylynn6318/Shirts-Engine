#include "shader/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

se::Shader::Shader(): shaderProgram(0), fragmentShader(0), vertexShader(0)
{

}

se::Shader::~Shader()
{

}

auto se::Shader::loadShader(const char* vertPath, const char* fragPath)->void
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertFile.rdbuf();
        fShaderStream << fragFile.rdbuf();
        vertFile.close();
        fragFile.close();
        vertCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    // 2. compile shaders
    GLuint vert, frag;
    // vertex shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glCompileShader(vert);
    checkCompileErrors(vert, ShaderType::VERTEX);
    // fragment Shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);
    checkCompileErrors(frag, ShaderType::FRAGMENT);
    // shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, ShaderType::PROGRAM);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vert);
    glDeleteShader(frag);
}

auto se::Shader::unLoadShader()->void
{
    glDeleteProgram(shaderProgram);
}

auto se::Shader::activeShader()->void
{
    glUseProgram(shaderProgram);
}

auto se::Shader::checkCompileErrors(unsigned int shader, const ShaderType& type) -> void
{
    GLint success;
    const unsigned int SIZE = 1024;
    char infoLog[SIZE];
    if (type != ShaderType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, SIZE, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << static_cast<int>(type) << "\n"
                << infoLog << "\n -- ------------------------------------------------------ " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, SIZE, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << static_cast<int>(type) << "\n"
                << infoLog << "\n -- ------------------------------------------------------ " << std::endl;
        }
    }
}

auto se::Shader::setBool(const char* name, bool value) const -> void
{
    glUniform1i(glGetUniformLocation(shaderProgram, name), (int)value);
}
auto se::Shader::setInt(const char* name, int value) const -> void
{
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}
auto se::Shader::setFloat(const char* name, float value) const -> void
{
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}
auto se::Shader::setVec2(const char* name, const glm::vec2& value) const -> void
{
    glUniform2fv(glGetUniformLocation(shaderProgram, name), 1, &value[0]);
}
auto se::Shader::setVec2(const char* name, float x, float y) const -> void
{
    glUniform2f(glGetUniformLocation(shaderProgram, name), x, y);
}
auto se::Shader::setVec3(const char* name, const glm::vec3& value) const -> void
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &value[0]);
}
auto se::Shader::setVec3(const char* name, const sem::Vector3& value) const -> void
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, value.getAsFloatPtr());
}
auto se::Shader::setVec3(const char* name, float x, float y, float z) const -> void
{
    glUniform3f(glGetUniformLocation(shaderProgram, name), x, y, z);
}
auto se::Shader::setVec4(const char* name, const glm::vec4& value) const -> void
{
    glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, &value[0]);
}
auto se::Shader::setVec4(const char* name, float x, float y, float z, float w) const -> void
{
    glUniform4f(glGetUniformLocation(shaderProgram, name), x, y, z, w);
}
auto se::Shader::setMat2(const char* name, const glm::mat2& mat) const -> void
{
    glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &mat[0][0]);
}
auto se::Shader::setMat3(const char* name, const glm::mat3& mat) const -> void
{
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &mat[0][0]);
}
auto se::Shader::setMat4(const char* name, const glm::mat4& mat) const -> void
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &mat[0][0]);
}
auto se::Shader::setMat4(const char* name, const sem::Matrix4& mat) const -> void
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, mat.getAsFloatPtr());
}
