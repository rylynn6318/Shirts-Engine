#include <iostream>

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
