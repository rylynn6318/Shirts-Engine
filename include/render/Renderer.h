#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

	private:
		GLFWwindow* window;
	};
}