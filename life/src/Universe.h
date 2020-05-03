#pragma once
#include <SFML/Graphics.hpp>

#include <set>
#include <vector>
#include <utility>

#define LENGTH 1024
#define HIGHT 768
#define CELL_SIZE 8 // Remember: HIGH % CELL_SIZE && LENGTH % CELL_SIZE must be 0 

struct Universe
{
	struct Cell
	{
		Cell(int x_, int y_, int Alive)
		{
			isAlive = Alive;
			x = x_;
			y = y_;
		}
		int isAlive;
		int x;
		int y;
		friend bool operator==(const Cell& lhs, const Cell& rhs);
	};
	//It is more effective to use std::set for search, however std::vector is used for storage
	//information about all cells(include dead ones) and then sends it to master proccess
	std::vector<Cell> field;
	std::set<std::pair<int, int>> coordinates;
};


inline bool operator==(const Universe::Cell & lhs, const Universe::Cell & rhs)
{
	if ((rhs.x == lhs.x) && (lhs.y == rhs.y))
		return true;
	return false;
}