#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader/Shader.h"
#include "math/Math.h"
#include "render/StaticModel.h"
#include "render/Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace sem = se::math;

namespace se
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = default;
		auto init(int SCR_WIDTH, int SCR_HEIGHT)->bool;
		auto draw()->void;
		auto terminate()->void;
		auto setLightUniforms(se::Shader& shader, const sem::Matrix4& viewMat)->void;
		auto setLightUniforms(se::Shader& shader)->void;

	private:
		GLFWwindow* window;
		Shader textureShader;
		Shader staticMeshShader;
		Shader skeletalMeshShader;

		GLuint testVAO;
		GLuint testVBO;
		GLuint testTexture;

		StaticModel staticModel;

		sem::Matrix4 view;
		sem::Matrix4 projection;
		sem::Vector3 ambientLight;
	};
}