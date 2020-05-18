#include "enemy.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <memory>
#include <time.h>
#include <thread>

void RedEnemy::AI_Move(Grid& grid, Player& player, std::promise<void>&& prms){
	// red enemy uses a-star algo to chase
	RunAStarSearchAndMove(x, y, player.x, player.y, grid);
	prms.set_value();
}

void BlueEnemy::AI_Move(Grid& grid, Player& player, std::promise<void>&& prms){
	// blue enemy uses random moves
	RunRandomAlgorithmAndMove(grid);
	prms.set_value();
}

void BlueEnemy::RunRandomAlgorithmAndMove(Grid& grid){
	// delta for all possible moves on the grid
	int delta[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
	std::vector<std::vector<int>> moves;
	for(int i = 0; i < 4; i++){
		int x2 = x + delta[i][0];
		int y2 = y + delta[i][1];
		// for each neighbor cell..
		if(Random_valid_cell(x2, y2, grid)){
			moves.push_back(std::vector<int>{x2, y2});
		}
	}
	if(moves.size() == 0){ 
		std::cerr << "Error: Blue Enemy has no moves left!\n";
		return;
	}
	// select one of the valid moves at random
	// the pseudo random generator is seeded at start of game 
	int random_index = rand() % (moves.size());
	// record prev position
	prev_x = x;
	prev_y = y;
	// move
	x = moves[random_index][0];
	y = moves[random_index][1];
}

bool BlueEnemy::Random_valid_cell(int x, int y, Grid& grid){
	// acquire grid mutex
	std::lock_guard<std::mutex> lck(grid.mtx);

	// check if out of grid
	if((x < 0) || (x > 18)){ return false; }
	if((y < 0) || (y > 22)){ return false; }

	// check for grid walls
	if(grid.at(x, y) == Grid::GridElement::kWall){ return false; }

	// prevent move to previous cell
	if((x == prev_x) && (y == prev_y)){ return false; }

	// prevent move into tunnel leading to portal
	if((x == 3) && (y == 10)){ return false; }
	if((x == 15) && (y == 10)){ return false; }

	// else it is valid cell to move
	return true;	
}

void RedEnemy::RunAStarSearchAndMove(int start_x, int start_y, int goal_x, int goal_y, Grid& grid){
	// delta for all possible moves on the grid
	int delta[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

	// keeping track of expanded nodes
	int visited_nodes[19][23];
	memset(visited_nodes, 0, sizeof(visited_nodes));

	std::vector<std::shared_ptr<Node>> openlist;

	std::shared_ptr<Node> start_node (new Node); 
	start_node->x = start_x;
	start_node->y = start_y;
	start_node->g = 0;
	start_node->h = Heuristic(start_node->x, start_node->y, goal_x, goal_y);
	start_node->parent = nullptr;

	// push node into openlist and mark it to prevent readdition
	openlist.push_back(start_node);
	visited_nodes[start_node->x][start_node->y] = 1;	
	
	std::shared_ptr<Node> current_node = nullptr; 

	while(openlist.size() > 0){
		// sort the open list and get the lowest f-value(g+h) node to expand next
		std::sort(openlist.begin(), openlist.end(), [](std::shared_ptr<Node> a, std::shared_ptr<Node> b){ return (a->g + a->h) < (b->g + b->h); });
		current_node = openlist.front();
		openlist.erase(openlist.begin());

		// check if we found the goal node and record the path
		if((current_node->x == goal_x) && (current_node->y == goal_y)){
			std::shared_ptr<Node> ptr = current_node;
			ai_path.clear();
			// find the first node in path excluding the starting node
			if(ptr->parent){
				while(ptr->parent->parent != nullptr){
					ptr = ptr->parent;
					ai_path.push_back(std::vector<int>{ptr->x, ptr->y});
				}
			}
			// move to the first node in path excluding starting node
			x = ptr->x;
			y = ptr->y;
			return;
		}

		// if goal node is not found, expand the current node to find its neighbors
		for(int i = 0; i < 4; i++){
			int x2 = current_node->x + delta[i][0];
			int y2 = current_node->y + delta[i][1];
			// for each neighbor cell..
			if(AStar_CheckValidCell(x2, y2, grid, visited_nodes)){
				std::shared_ptr<Node> node (new Node); 
				node->x = x2;
				node->y = y2;
				node->g = current_node->g + 1;
				node->h = Heuristic(node->x, node->y, goal_x, goal_y);
				node->parent = current_node;
				// add the valid neighbor
				openlist.push_back(node);
				visited_nodes[node->x][node->y] = 1;
			} 
		}	
	}
}

int RedEnemy::Heuristic(int x1, int y1, int x2, int y2){
	return std::abs(x2 - x1) + std::abs(y2 - y1);
}

bool RedEnemy::AStar_CheckValidCell(int x, int y, Grid& grid, int(&visited_nodes)[19][23]){
	// acquire grid mutex
	std::lock_guard<std::mutex> lck(grid.mtx);

	// check if out of grid
	if((x < 0) || (x > 18)){ return false; }
	if((y < 0) || (y > 22)){ return false; }

	// check if node is already visited
	if(visited_nodes[x][y] == 1){ return false; }

	// check for grid wall
	if(grid.at(x, y) == Grid::GridElement::kWall){ return false; }
	
	// else it is valid node
	return true;
}

