#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/Shader.h"
#include "render/VertexArray.h"


namespace se
{
	class Game
	{
	public:
		Game();
		~Game();

		auto init()->bool;
		auto run()->void;
		auto terminate()->void;

	public:


	private:
		Shader shader;
		VertexArray* vao;
		void processInput(GLFWwindow* window);
		GLFWwindow* window;
		void load(); //test for load shader, vertex
		bool isRunning = true;
	};
}