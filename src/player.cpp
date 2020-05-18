#include "player.h"
#include "grid.h"

void Player::Move(Grid& grid){
	// acquire grid mutex
	std::lock_guard<std::mutex> lck(grid.mtx);
	// check for wall before moving
	switch(direction){
		case Direction::kUp: 
			y = (grid.at(x, y-1) == Grid::GridElement::kWall)? y : (y - 1); 
			break;

		case Direction::kDown: 
			y = (grid.at(x, y+1) == Grid::GridElement::kWall)? y : (y + 1); 
			break;

		case Direction::kLeft: 
			x = (grid.at(x-1, y) == Grid::GridElement::kWall)? x : (x - 1); 
			break;

		case Direction::kRight: 
			x = (grid.at(x+1, y) == Grid::GridElement::kWall)? x : (x + 1); 
			break;
	}

	// check for portal
	if(x == 0 && y == 10){ x = 17; }
	if(x == 18 && y == 10){ x = 1; }

	// check for food and eat it
	if(grid.at(x, y) == Grid::GridElement::kFood){
		score++;
		grid.set(x, y, Grid::GridElement::kEmpty);
	}
}