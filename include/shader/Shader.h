#pragma once
#include <string>
#include "glad/glad.h" 
#include "glm/glm.hpp"

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
		auto setVec2(const std::string& name, const glm::vec2& value) const -> void;
		auto setVec2(const std::string& name, float x, float y) const -> void;
		auto setVec3(const std::string& name, const glm::vec3& value) const -> void;
		auto setVec3(const std::string& name, float x, float y, float z) const -> void;
		auto setVec4(const std::string& name, const glm::vec4& value) const -> void;
		auto setVec4(const std::string& name, float x, float y, float z, float w) const -> void;
		auto setMat2(const std::string& name, const glm::mat2& mat) const -> void;
		auto setMat3(const std::string& name, const glm::mat3& mat) const -> void;
		auto setMat4(const std::string& name, const glm::mat4& mat) const -> void;

		GLuint getShaderPrgram() const { return shaderProgram; }
	private:
		auto compileShader(const std::string& file)->bool;
		auto checkCompileErrors(unsigned int shader, const std::string& type) -> void;

		GLuint shaderProgram;
		GLuint fragmentShader;
		GLuint vertextShader;
	};
}