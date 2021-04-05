#include "render/Renderer.h"
#include <iostream>
#include <chrono>
namespace se
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		
	}
}

auto se::Renderer::init(int SCR_WIDTH, int SCR_HEIGHT)-> bool
{
	glfwInit();
	//openGL 4.6 version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shirts & Jean", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	staticMeshShader.loadShader("../resource/shaders/4.6.phong.vert", "../resource/shaders/4.6.phong.frag");
	//staticMeshShader.loadShader("../resource/shaders/4.6.model.vert.glsl", "../resource/shaders/4.6.model.frag.glsl");
	staticModel.loadModel("../resource/model/GrassBlock/Grass_Block.obj");
	return true;
}

auto se::Renderer::draw()->void
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//텍스쳐

	//textureShader.activeShader();
	//스태틱메시
	staticMeshShader.activeShader();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(5,5,5), glm::vec3(0,0,0), glm::vec3(0, 1, 0));

	//MVP,계산은 역순
	staticMeshShader.setMat4("uViewProj", projection * view);

	//TRS,트랜스폼 적용
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
	staticMeshShader.setMat4("uWorldTransform", transform);

	staticMeshShader.setFloat("uSpecPower", 128.0f);  

	setLightUniforms(staticMeshShader);
	staticModel.draw(staticMeshShader);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
	staticMeshShader.setMat4("uWorldTransform", transform);
	staticModel.draw(staticMeshShader);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
	transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
	staticMeshShader.setMat4("uWorldTransform", transform);
	staticModel.draw(staticMeshShader);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 2.0f));
	transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
	staticMeshShader.setMat4("uWorldTransform", transform);
	staticModel.draw(staticMeshShader);

	//스켈레탈메시
	//skeletalMeshShader.activeShader();
	//조명

	glfwSwapBuffers(window);
	glfwPollEvents();
}

auto se::Renderer::setLightUniforms(se::Shader& shader, const sem::Matrix4& viewMat)->void
{
	sem::Matrix4 view = viewMat;
	view.invert();
	//shader.setVec3("cameraPos", view.getTranslation());
	//shader.setVec3("uAmbientLight", ambientLight);
	//shader.setVec3("uDirLight.mDirection", directionLight.direction);
	//shader.setVec3("uDirLight.mDiffuseColor", directionLight.diffuseColor);
	//shader.setVec3("uDirLight.mSpecColor", directionLight.specularColor);
}

auto se::Renderer::setLightUniforms(se::Shader& shader)->void
{
	shader.setVec3("cameraPos", glm::vec3(5, 5, 5));
	shader.setVec3("uAmbientLight", glm::vec3(0.4f, 0.4f, 0.4f));
	shader.setVec3("uDirLight.direction", glm::vec3(0.7f * sin(glfwGetTime()), 0.7f, 0.7f * cos(glfwGetTime())));
	shader.setVec3("uDirLight.diffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setVec3("uDirLight.specColor", glm::vec3(0.5f, 1.0f, 0.5f));
}