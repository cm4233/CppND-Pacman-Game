#pragma once
#include "grid.h"

class Player{
	public:
		enum Direction{ kRight = 0, kDown = 1, kUp = 2, kLeft = 3 };
		Direction direction = Direction::kUp;
		// position
		int x = 1;
		int y = 1;

		// frame for render animation
		int frame = 0;

		// function to move
		void Move(Grid& grid);
		int getScore(){ return score; }
	private:
		int score = 0;
};