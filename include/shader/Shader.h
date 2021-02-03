#pragma once
#include <string>
#include "glad/glad.h" 

namespace se
{
	class Shader
	{
	public:
		Shader();
		~Shader();
		auto loadShader(const std::string& vertPath, const std::string& fragPath)->void;
		auto unLoadShader()->void;
		auto activeShader()->void;
		auto setBool(const std::string& name, bool value) const -> void;
		auto setInt(const std::string& name, int value) const -> void;
		auto setFloat(const std::string& name, float value) const -> void;
		GLuint getShaderPrgram() const { return shaderProgram; }
	private:
		auto compileShader(const std::string& file)->bool;
		auto checkCompileErrors(unsigned int shader, const std::string& type) -> void;

		GLuint shaderProgram;
		GLuint fragmentShader;
		GLuint vertextShader;
	};
}