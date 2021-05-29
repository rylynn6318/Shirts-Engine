#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "entity/EntityDB.h"
#include "shader/Shader.h"
#include "math/Math.h"
#include "render/StaticModel.h"
#include "render/Mesh.h"
#include "render/CameraComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <component/StaticModelComponent.h>

namespace sem = se::math;
using namespace std::chrono_literals;

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

        auto drawStaticMeshInit() -> void {
            //MVP,계산은 역순
            this->staticMeshShader.activeShader();
            this->staticMeshShader.setMat4("uViewProj", renderer_camera.activeCamera().getProjectionMatrix() * renderer_camera.activeCamera().getViewMatrix());

            this->staticMeshShader.setFloat("uSpecPower", 128.0f);
            this->setLightUniforms(this->staticMeshShader);
        };
        auto drawStaticMesh(se::TransformComponent &trans, se::StaticModelComponent &model) -> void {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans.position);
            // quaternion 어케 쓰지?
            transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            transform = glm::scale(transform, trans.scale);
            this->staticMeshShader.setMat4("uWorldTransform", transform);
            model.model->draw(this->staticMeshShader);
        }

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

        CameraComponent renderer_camera{};

		//CameraComponent camera{ glm::vec3(5.0f,5.0f,5.0f) };
        void drawStaticMesh(EntityDB &edb);
        void drawSkybox();
        void drawTextures();
    };
}