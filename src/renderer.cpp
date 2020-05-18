#include "renderer.h"
#include <iostream>

Renderer::Renderer(){	

	// Initialize SDL
	if( SDL_InitSubSystem(SDL_INIT_VIDEO) < 0 ){ 
		std::cerr << "Could not initialize SDL.\n"; 
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}

	// Create window
	sdl_window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (sdl_window == nullptr) {
    	std::cerr << "Window could not be created.\n";
    	std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  	}

  	// Create renderer
  	sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  	if (sdl_renderer == nullptr) {
    	std::cerr << "Renderer could not be created.\n";
    	std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  	}

  	// Load pacman's spritesheet as texture for rendering
  	SDL_Surface* image = SDL_LoadBMP("../data/pacman50x50spritesheet.bmp");
	if(image == nullptr){
		std::cerr << "Unable to load spritesheet bmp\n";
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}
	pacman_spritesheet_texture = SDL_CreateTextureFromSurface(sdl_renderer, image);
	if(pacman_spritesheet_texture == nullptr){
		std::cerr << "Unable to create spritesheet texture\n";
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}
	SDL_FreeSurface(image); 
	image = nullptr;
}

Renderer::~Renderer(){
	// RAII
	SDL_DestroyTexture(pacman_spritesheet_texture); 
	pacman_spritesheet_texture = nullptr;

	SDL_DestroyWindow(sdl_window); 
	sdl_window = nullptr;

	SDL_DestroyRenderer(sdl_renderer); 
	sdl_renderer = nullptr;

	SDL_Quit();
}

void Renderer::RenderGameStateToWindow(Grid& grid, Player& player, RedEnemy& enemy, BlueEnemy& enemy2){
	// clear screen
	SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255); // white
	SDL_RenderClear(sdl_renderer);
	// draw game grid
	DrawGameGrid(grid);
	// indicate grid's portals
	DrawGridPortals();
	// draw the player
	DrawPlayer(player);
	// draw enemy
	DrawRedEnemy(enemy);
	DrawBlueEnemy(enemy2);
	// display the final render
	SDL_RenderPresent(sdl_renderer);
}

void Renderer::DrawGameGrid(Grid& grid){
	SDL_Rect sdl_rect;
	for(int i = 0; i < 19; i++){
		for(int j = 0; j < 23; j++){
			// draw grid's wall cells
			if( grid.at(i, j) == Grid::GridElement::kWall ){
				sdl_rect.x = i * tile_width; 
				sdl_rect.y = j * tile_width;
				sdl_rect.w = tile_width; 
				sdl_rect.h = tile_width;
				SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255); // black
				SDL_RenderFillRect(sdl_renderer, &sdl_rect);
				continue;
			}

			// draw grid's food cells
			if( grid.at(i, j) == Grid::GridElement::kFood ){
				sdl_rect.x = (i * tile_width) + ((tile_width*3)/8); 
				sdl_rect.y = (j * tile_width) + ((tile_width*3)/8);
				sdl_rect.w = ((tile_width*1)/4); 
				sdl_rect.h = ((tile_width*1)/4);
				SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255); // black
				SDL_RenderFillRect(sdl_renderer, &sdl_rect);
				continue;
			}

			// draw grid's empty cells
			if( grid.at(i, j) == Grid::GridElement::kEmpty ){
				sdl_rect.x = i * tile_width; 
				sdl_rect.y = j * tile_width;
				sdl_rect.w = tile_width; 
				sdl_rect.h = tile_width;
				SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255); // white
				SDL_RenderFillRect(sdl_renderer, &sdl_rect);
				continue;
			}
		}
	}
}

void Renderer::DrawGridPortals(){
	SDL_Rect sdl_rect;
	
	SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 0, 255); // green
	sdl_rect.x = 0 * tile_width;
	sdl_rect.y = 10 * tile_width;
	sdl_rect.w = tile_width;
	sdl_rect.h = tile_width;
	SDL_RenderFillRect(sdl_renderer, &sdl_rect);
	sdl_rect.x = 18 * tile_width;
	sdl_rect.y = 10 * tile_width;
	sdl_rect.w = tile_width;
	sdl_rect.h = tile_width;
	SDL_RenderFillRect(sdl_renderer, &sdl_rect);
}

void Renderer::DrawPlayer(Player& player){
	SDL_Rect src_rect;
	SDL_Rect dst_rect;

	player.frame = (player.frame + 1) % 2;
	src_rect.x = player.frame * 50;
	src_rect.y = player.direction * 50;
	src_rect.w = 50; 
	src_rect.h = 50;
	dst_rect.x = player.x * tile_width; 
	dst_rect.y = player.y * tile_width; 
	dst_rect.w = tile_width; 
	dst_rect.h = tile_width;
	SDL_RenderCopy(sdl_renderer, pacman_spritesheet_texture, &src_rect, &dst_rect);
}

void Renderer::DrawRedEnemy(RedEnemy& enemy){
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	SDL_Rect sdl_rect;

	enemy.frame = (enemy.frame + 1) % 16;
	src_rect.x = 2 * 50;
	src_rect.y = (enemy.frame/4) * 50;
	src_rect.w = 50; 
	src_rect.h = 50;
	dst_rect.x = enemy.x * tile_width; 
	dst_rect.y = enemy.y * tile_width; 
	dst_rect.w = tile_width; 
	dst_rect.h = tile_width;
	SDL_RenderCopy(sdl_renderer, pacman_spritesheet_texture, &src_rect, &dst_rect);
	// draw enemy's ai_path
	if (enemy.ai_path.size()){
		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255); // red
		for(int i = 0; i < enemy.ai_path.size() - 1; i++){
			sdl_rect.x = (enemy.ai_path[i])[0] * tile_width + 5; 
			sdl_rect.y = (enemy.ai_path[i])[1] * tile_width + 5;;
			sdl_rect.w = tile_width - (2 * 5); 
			sdl_rect.h = tile_width - (2 * 5);
			SDL_RenderDrawRect(sdl_renderer, &sdl_rect);
		}
	}
}

void Renderer::DrawBlueEnemy(BlueEnemy& enemy){
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	SDL_Rect sdl_rect;

	src_rect.x = 3 * 50;
	src_rect.y = 0 * 50;
	src_rect.w = 50; 
	src_rect.h = 50;
	dst_rect.x = enemy.x * tile_width; 
	dst_rect.y = enemy.y * tile_width; 
	dst_rect.w = tile_width; 
	dst_rect.h = tile_width;
	SDL_RenderCopy(sdl_renderer, pacman_spritesheet_texture, &src_rect, &dst_rect);
}
