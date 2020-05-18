#pragma once
#include "SDL.h"
#include "grid.h"
#include "player.h"
#include "enemy.h"

// RAII class
class Renderer{
	public:
		Renderer();
		~Renderer();
		// Rule of Five
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&&) = delete;

		void RenderGameStateToWindow(Grid& grid, Player& player, RedEnemy& enemy, BlueEnemy& enemy2);
	private:
		const std::size_t tile_width = 30;
		const std::size_t screen_height = 23 * tile_width;
		const std::size_t screen_width = 19 * tile_width;

		SDL_Window* sdl_window = nullptr;
		SDL_Renderer* sdl_renderer = nullptr; 
		SDL_Texture* pacman_spritesheet_texture = nullptr;

		void DrawGameGrid(Grid& grid);
		void DrawGridPortals();
		void DrawPlayer(Player& player);
		void DrawRedEnemy(RedEnemy& enemy);
		void DrawBlueEnemy(BlueEnemy& enemy);
};
