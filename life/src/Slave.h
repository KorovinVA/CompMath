#pragma once
#include <algorithm>
#include <cstdlib>

#include <set>
#include "Universe.h"

class Slave
{
	struct TransmittedArrays
	{
		int* cells;
		int* MyLstr;
		int* OthLstr;
		int* MyRstr;
		int* OthRstr;
	} arrays;

	int transmittSize;
	int m_rank;
	int m_size;
	int m_part;
	Universe m_world;

	void AllocateSideArrays();
	void CalculateNeighbor(std::pair<int, int> coord, int& count);
	void putInSideArrays();
	void Communicate();
	bool CreateNextGen();
	bool Calcs(int x, int y, int& count, bool isCurCellAlive);
public:
	Slave(int Prank, int Psize);
	void run();
	~Slave();
};