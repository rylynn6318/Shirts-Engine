#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader/Shader.h"
#include "math/Math.h"

namespace sem = se::math;

namespace se
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		auto init(int SCR_WIDTH, int SCR_HEIGHT)->bool;
		auto draw()->void;
		auto terminate()->void;
		auto setLightUniforms(se::Shader& shader, const sem::Matrix4& viewMat)->void;

	private:
		GLFWwindow* window;
		Shader textureShader;
		Shader staticMeshShader;
		Shader skeletalMeshShader;

		sem::Matrix4 view;
		sem::Matrix4 projection;

		sem::Vector3 ambientLight;
	};
}