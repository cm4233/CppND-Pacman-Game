#include <iostream>
#include "grid.h"
#include "renderer.h"
#include "controller.h"
#include "game.h"

int main(){
	Renderer renderer;
	Controller controller;
	Game game;
	game.StartGameLoop(controller, renderer);
	std::cout << "--Game terminated successfully--\n";
	return 0;
}