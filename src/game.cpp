#include "game.h"
#include "SDL.h"
#include <iostream>
#include <time.h>
#include <thread>
#include <future>
#define USER_REACTION_MS 200

void Game::StartGameLoop(Controller& controller, Renderer& renderer){
	// seeding the pseudo random number generator rand()
	srand(time(NULL));
	// game loop
	while(running){
		frame_start = SDL_GetTicks();

		// 1. Input : handle user input and change player's direction
		controller.HandleUserInputAndChangeDirection(running, player, grid);

		// 2. Update : use player's direction to move and change game state
		MovePlayer();
		if(running){
			AIMoveEnemies();
		}
		
		// 3. Render : show above changes on screen
		renderer.RenderGameStateToWindow(grid, player, enemy, enemy2);

		// 4. Timing : wait (USER_REACTION_MS) ms for user to react before the next update & render
		frame_end = SDL_GetTicks();
		frame_duration = frame_end - frame_start;
		//std::cout << frame_duration << "\n";
		if(frame_duration < USER_REACTION_MS){ 
			SDL_Delay(USER_REACTION_MS - frame_duration); 
		}
		if(!running){ SDL_Delay(1500); }
	}
}

void Game::CheckForWinCondition(bool& running, Player& player){
	if(player.getScore() == 190){
		std::cout << "You win!\n";
		running = false;
	}
}

void Game::CheckForCollision(bool& running){
	if(( (player.x == enemy.x)&&(player.y == enemy.y) ) || ( (player.x == enemy2.x)&&(player.y == enemy2.y) )){
		running = false;
		std::cout << "You were captured! Try again.\n";
	}
}

void Game::MovePlayer(){
	player.Move(grid);
	CheckForWinCondition(running, player);
	CheckForCollision(running);
}

void Game::AIMoveEnemies(){
	// thread 1 for red enemy to compute next move
	std::promise<void> prms1;
	std::future<void> ftr1 = prms1.get_future();
	std::thread t1(&RedEnemy::AI_Move, &enemy, std::ref(grid), std::ref(player), std::move(prms1));
	
	// thread 2 for blue enemy to compute next move
	std::promise<void> prms2;
	std::future<void> ftr2 = prms2.get_future();
	std::thread t2(&BlueEnemy::AI_Move, &enemy2, std::ref(grid), std::ref(player), std::move(prms2));
	
	// wait before checking collision
	ftr1.wait();
	ftr2.wait();
	CheckForCollision(running);
	
	// thread barrier
	t1.join();
	t2.join();
}