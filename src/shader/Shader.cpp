#include "shader/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

se::Shader::Shader(): shaderProgram(0), fragmentShader(0), vertextShader(0)
{

}

se::Shader::~Shader()
{

}

auto se::Shader::loadShader(const std::string& vertPath, const std::string& fragPath)->void
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertFile.open(vertPath.c_str());
        fragFile.open(fragPath.c_str());
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
    checkCompileErrors(vert, "VERTEX");
    // fragment Shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);
    checkCompileErrors(frag, "FRAGMENT");
    // shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vert);
    glDeleteShader(frag);

}

auto se::Shader::unLoadShader()->void
{
    glDeleteProgram(shaderProgram);
}

auto se::Shader::useShader()->void
{
    glUseProgram(shaderProgram);
}

auto se::Shader::checkCompileErrors(unsigned int shader, const std::string& type) -> void
{
    GLint success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

auto se::Shader::setBool(const std::string& name, bool value) const -> void
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.data()), (int)value);
}
auto se::Shader::setInt(const std::string& name, int value) const -> void
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.data()), value);
}
auto se::Shader::setFloat(const std::string& name, float value) const -> void
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.data()), value);
}