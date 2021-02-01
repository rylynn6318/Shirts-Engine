#pragma once
#include <string>

namespace se
{
    class Shader
    {
    public:
        unsigned int ID;
        // constructor generates the shader on the fly
        Shader(const char *vertexPath, const char *fragmentPath);

        // activate the shader
        auto use() -> void;

        // utility uniform functions
        auto setBool(const std::string &name, bool value) const -> void;
        auto setInt(const std::string &name, int value) const -> void;
        auto setFloat(const std::string &name, float value) const -> void;

    private:
        // utility function for checking shader compilation/linking errors.
        auto checkCompileErrors(unsigned int shader, std::string type) -> void;
    };
} // namespace se