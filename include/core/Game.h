#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

	private:
		GLFWwindow* window;
	};
}