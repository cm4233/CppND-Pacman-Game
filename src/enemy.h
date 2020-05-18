#pragma once
#include "grid.h"
#include "player.h"
#include <vector>
#include <memory>
#include <future>

class Enemy{
	public:
		// constructor
		Enemy(int a, int b): x(a), y(b){}
		// position
		int x;
		int y;
		
		// virtual function to move enemy 
		virtual void AI_Move(Grid& grid, Player& player, std::promise<void>&& prms) = 0;
};

// blue enemy uses random algorithm to move
class BlueEnemy : public Enemy{
	public:
		BlueEnemy(int a, int b): Enemy(a, b){}
		void AI_Move(Grid& grid, Player& player, std::promise<void>&& prms) override;
	private:
		int prev_x = 0;
		int prev_y = 0;
		void RunRandomAlgorithmAndMove(Grid& grid);
		bool Random_valid_cell(int x, int y, Grid& grid);
};

// red enemy uses a-star search (player chase) to move
class RedEnemy : public Enemy{
	public:
		RedEnemy(int a, int b): Enemy(a, b){}
		void AI_Move(Grid& grid, Player& player, std::promise<void>&& prms) override;
		// node for a-star search algorithm
		struct Node{
			int x;
			int y;
			int g;
			int h;
			std::shared_ptr<Node> parent = nullptr;
		};

		// save the ai path for the renderer to show
		std::vector<std::vector<int>> ai_path;

		// frame for render animation
		int frame = 0;
	private:
		// functions for a-star algorithm
		void RunAStarSearchAndMove(int start_x, int start_y, int goal_x, int goal_y, Grid& grid);
		int Heuristic(int x1, int y1, int x2, int y2);
		bool AStar_CheckValidCell(int x, int y, Grid& grid, int(&visited_nodes)[19][23]);
};