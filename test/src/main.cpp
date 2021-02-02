#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/Shader.h"
#include <iostream>
#include "render/VertexArray.h"
#include "core/Game.h"


int main()
{
	se::Game game;
	if (game.init())
	{
		game.run();
			
	}


	glfwTerminate();
	return 0;
}
