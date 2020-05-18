#include "SDL.h"
#include "controller.h"

void Controller::HandleUserInputAndChangeDirection(bool& running, Player& player, Grid& grid){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		// event where user X the window to close
		if(event.type == SDL_QUIT){ running = false; }
		// keyboard event
		if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				// check for walls before changing player's direction
				case SDLK_UP:
					player.direction = (grid.at(player.x, player.y-1) == Grid::GridElement::kWall)? player.direction : Player::Direction::kUp;
					break;

				case SDLK_DOWN: 
					player.direction = (grid.at(player.x, player.y+1) == Grid::GridElement::kWall)? player.direction : Player::Direction::kDown; 
					break;

				case SDLK_LEFT: 
					player.direction = (grid.at(player.x-1, player.y) == Grid::GridElement::kWall)? player.direction : Player::Direction::kLeft; 
					break;

				case SDLK_RIGHT: 
					player.direction = (grid.at(player.x+1, player.y) == Grid::GridElement::kWall)? player.direction : Player::Direction::kRight; 
					break;
			}
		}
	}
}
