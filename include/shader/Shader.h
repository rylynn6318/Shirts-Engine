#pragma once
#include <string>
#include "glad/glad.h" 
#include "glm/glm.hpp"
#include "math/Math.h"
namespace sem = se::math;
namespace se
{
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	class Shader
	{
	public:
		Shader();
		~Shader();
		auto loadShader(const const char* vertPath, const const char* fragPath)->void;
		auto unLoadShader()->void;
		auto activeShader()->void;

		auto setBool(const char* name, bool value) const -> void;
		auto setInt(const char* name, int value) const -> void;
		auto setFloat(const char* name, float value) const -> void;

		auto setVec2(const char* name, const glm::vec2& value) const -> void;
		auto setVec2(const char* name, float x, float y) const -> void;

		auto setVec3(const char* name, const glm::vec3& value) const -> void;
		auto setVec3(const char* name, const sem::Vector3& value) const -> void;
		auto setVec3(const char* name, float x, float y, float z) const -> void;

		auto setVec4(const char* name, const glm::vec4& value) const -> void;
		auto setVec4(const char* name, float x, float y, float z, float w) const -> void;

		auto setMat2(const char* name, const glm::mat2& mat) const -> void;
		auto setMat3(const char* name, const glm::mat3& mat) const -> void;
		auto setMat4(const char* name, const glm::mat4& mat) const -> void;
		auto setMat4(const char* name, const sem::Matrix4& mat) const -> void;

		GLuint getShader() const { return shaderProgram; }
	private:
		auto compileShader(const char* file)->bool;
		auto checkCompileErrors(unsigned int shader, const ShaderType& type) -> void;

		GLuint shaderProgram;
		GLuint fragmentShader;
		GLuint vertextShader;
	};
}