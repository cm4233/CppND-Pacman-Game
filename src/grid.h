#pragma once
#include <thread>
#include <mutex>

class Grid{
	public:
		enum GridElement{kEmpty = 0, kWall = 1, kFood = 2};
		Grid();
		// getter
		int at(int x, int y);
		// setter
		void set(int x, int y, int val);
		// grid mutex to sync access
		std::mutex mtx;
	private:
		int grid[23][19];
		// load grid from file
		void LoadGameGrid();
		// print grid for debug purpose
		void PrintGameGrid();
};