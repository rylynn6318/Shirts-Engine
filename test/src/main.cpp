#include <iostream>
#include "core/Game.h"

int main()
{
	se::Game game;
	if (game.init())
	{
		game.run();
	}
	game.terminate();

	return 0;
}
