#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/Shader.h"
#include "render/VertexArray.h"


namespace se
{
	//TODO : 메모리 해제는 나중에 구조가 정해지고 나서 바꿔야함
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