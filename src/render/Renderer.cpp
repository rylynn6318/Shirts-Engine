#include "render/Renderer.h"
#include <iostream>

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //default
	glEnable(GL_STENCIL_TEST);

	//textureShader.loadShader();
	staticMeshShader.loadShader("../resource/shaders/4.6.model.vert.glsl", "../resource/shaders/4.6.model.frag.glsl");
	//skeletalMeshShader.loadShader();


	return true;
}

auto se::Renderer::draw()->void
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//텍스쳐

	textureShader.activeShader();
	//스태틱메시
	staticMeshShader.activeShader();
	//스켈레탈메시
	skeletalMeshShader.activeShader();
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