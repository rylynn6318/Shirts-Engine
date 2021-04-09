#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader/Shader.h"
#include "math/Math.h"
#include "render/StaticModel.h"
#include "render/Mesh.h"
#include "render/Camera.h"
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
		auto processInput(GLFWwindow* window)->void;

	private:
		GLFWwindow* window;
		Shader textureShader;
		Shader staticMeshShader;
		Shader skeletalMeshShader;
		Shader skyboxShader;

		GLuint testVAO;
		GLuint testVBO;
		GLuint testEBO;
		GLuint testTexture;

		GLuint skyboxVAO;
		GLuint skyboxVBO;
		GLuint skyboxTexture;

		StaticModel staticModel;

		sem::Matrix4 view;
		sem::Matrix4 projection;
		sem::Vector3 ambientLight;

		//Camera camera{ glm::vec3(5.0f,5.0f,5.0f) };
        void drawStaticMesh(const glm::mat4 &projection, const glm::mat4 &view);

        void drawSkybox(const glm::mat4 &projection, const glm::mat4 &view);
    };
}