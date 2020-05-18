#pragma once
#include "controller.h"
#include "renderer.h"
#include "grid.h"
#include "player.h"

class Game{
	public:
		void StartGameLoop(Controller& controller, Renderer& renderer);
		int GetScore(){ return player.getScore(); }
		Grid grid;
		Player player;
		RedEnemy enemy{17, 21};
		BlueEnemy enemy2{17, 1};
	private:
		bool running = true;
		void MovePlayer();
		void AIMoveEnemies();
		void CheckForCollision(bool& running);
		void CheckForWinCondition(bool& running, Player& player);
		Uint32 frame_start;
		Uint32 frame_end;
		Uint32 frame_duration;
};