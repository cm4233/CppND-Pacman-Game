#pragma once
#include "player.h"
#include "grid.h"

class Controller
{
	public:
		void HandleUserInputAndChangeDirection(bool& running, Player& player, Grid& grid);
};
