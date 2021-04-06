#include "render/Renderer.h"
#include <iostream>
#include <vector>
#include <chrono>
#include "stb_image.h"

unsigned int loadTexture(char const* path);
unsigned int loadCubemap(std::vector<std::string> faces);

namespace se
{
	const unsigned int SCR_WIDTH = 1200;
	const unsigned int SCR_HEIGHT = 88;
	Camera camera{ glm::vec3(0.0f,0.0f,10.0f) };
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.processMouseMovement(xoffset, yoffset);
	}
}

float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
auto se::Renderer::init(int SCR_WIDTH, int SCR_HEIGHT) -> bool
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
	textureShader.loadShader("../resource/shaders/4.6.texture.vert", "../resource/shaders/4.6.texture.frag");
	skyboxShader.loadShader("../resource/shaders/4.6.skybox.vert", "../resource/shaders/4.6.skybox.frag");
	testTexture = loadTexture("../resource/image/pepe_thumsup.png");
	staticModel.loadModel("../resource/model/GrassBlock/Grass_Block.obj");


	glGenVertexArrays(1, &testVAO);
	glGenBuffers(1, &testVBO);
	glBindVertexArray(testVAO);
	glBindBuffer(GL_ARRAY_BUFFER, testVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces
	{
		"../resource/skybox/skybox/right.jpg",
		"../resource/skybox/skybox/left.jpg",
		"../resource/skybox/skybox/top.jpg",
		"../resource/skybox/skybox/bottom.jpg",
		"../resource/skybox/skybox/front.jpg",
		"../resource/skybox/skybox/back.jpg"
	};
	skyboxTexture = loadCubemap(faces);

	return true;
}

auto se::Renderer::draw()->void
{
	processInput(window);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1200 / (float)800, 0.1f, 1000.0f);
	//glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(2, 2, 2), glm::vec3(0, 1, 0));
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 10.0f, 0.0f));

	textureShader.activeShader();
	textureShader.setInt("texture1", 0);
	textureShader.setMat4("uView", view);
	textureShader.setMat4("uProj", projection);
	textureShader.setMat4("uModel", glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	glBindVertexArray(testVAO);
	glBindTexture(GL_TEXTURE_2D, testTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	model = glm::translate(model, glm::vec3(5.0f, 10.0f, -20.0f));
	textureShader.setMat4("uModel", glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	//스태틱메시



	//MVP,계산은 역순
	staticMeshShader.activeShader();
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

	

	// draw skybox as last
	glDepthFunc(GL_LEQUAL); 
	skyboxShader.activeShader();
	skyboxShader.setMat4("uView", view);
	skyboxShader.setMat4("uProj", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

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
	shader.setVec3("cameraPos", camera.Position);
	shader.setVec3("uAmbientLight", glm::vec3(0.4f, 0.4f, 0.4f));
	shader.setVec3("uDirLight.direction", glm::vec3(0.7f * sin(glfwGetTime()), 0.7f, 0.7f * cos(glfwGetTime())));
	shader.setVec3("uDirLight.diffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setVec3("uDirLight.specColor", glm::vec3(0.5f, 1.0f, 0.5f));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

auto se::Renderer::processInput(GLFWwindow* window)->void
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(se::CameraMovement::FORWARD, 0.1667f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(se::CameraMovement::BACKWARD, 0.1667f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(se::CameraMovement::LEFT, 0.1667f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(se::CameraMovement::RIGHT, 0.1667f);
}


unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}




