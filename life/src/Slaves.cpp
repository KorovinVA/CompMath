#include <mpi.h>

#include <iostream>
#include <algorithm>
#include <cstdlib>

#include <set>
#include "Universe.h"

struct TransmittedArrays
{
	int* cells;
	int* MyLstr;
	int* OthLstr;
	int* MyRstr;
	int* OthRstr;
};

void CalculateNeighbor(std::set<std::pair<int, int>>& values, std::pair<int, int> coord, int& count, bool isCurCellAlive);
bool CreateNextGen(Universe& World, int Part, int Psize, TransmittedArrays* arrays, int Prank);
TransmittedArrays AllocateArrays(int Part);
bool Calcs(std::set<std::pair<int, int>>& values, int x, int y, int& count, bool isCurCellAlive);
void DoNotRootWork(int Prank, int Psize);

void DoNotRootWork(int Prank, int Psize)
{
	int x = 0;
	int y = 0;
	Universe MyWorld;
	int Part = int(LENGTH / (Psize - 1));
	if (Prank == Psize - 1)
		Part += LENGTH % (Psize - 1);

	for (int i = 0; i < Part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			Universe::Cell cell(i, j, false);
			MyWorld.field.push_back(cell);
		}
	}
	while (x != -1 && y != -1)
	{
		MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		auto pos = std::find(MyWorld.field.begin(), MyWorld.field.end(), Universe::Cell(x, y, false));

		if (pos != MyWorld.field.end())
		{
			pos->isAlive = true;
			MyWorld.coordinates.insert({ x, y });
		}
	}
	TransmittedArrays arrays = AllocateArrays(Part);
	int isContinue = 0;
	do {
		isContinue = CreateNextGen(MyWorld, Part, Psize, &arrays, Prank);
	} while (isContinue != 0);

	free(arrays.cells);
}

TransmittedArrays AllocateArrays(int Part)
{
	int k = 0;
	for (int i = 0; i < Part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			k++;
		}
	}
	int* cells = (int*)calloc(k, sizeof(int));
	return { cells };
	/*int* leftString = nullptr;
int* rightString = nullptr;
if (Prank != 1 && Prank != Psize - 1)
{
	leftString = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
	rightString = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
}
else if (Prank == 1)
{
	rightString = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
}
else if (Prank == Psize - 1)
{
	leftString = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
}

int* PrevLstr = nullptr;
int* PrevRstr = nullptr;
if (Prank != 1)
{
	PrevLstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
	for (int i = 0; i < HIGHT / CELL_SIZE; ++i)
	{
		j = (World.field.size() - 1) - HIGHT / CELL_SIZE + i;
		if (World.field[j].isAlive)
		{
			PrevLstr[j] = 1;
		}
		else
		{
			PrevLstr[j] = 0;
		}
	}
}
if (Prank != Psize - 1)
{
	PrevRstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
	for (int i = 0; i < HIGHT / CELL_SIZE; ++i)
	{
		if (World.field[i].isAlive)
		{
			PrevRstr[i] = 1;
		}
		else
		{
			PrevRstr[i] = 0;
		}
	}
}

if (Prank == 1)
{
	MPI_Send
}*/
}

bool CreateNextGen(Universe& World, int Part, int Psize, TransmittedArrays* arrays, int Prank)
{
	for (int i = 0; i < Part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			auto pos = World.coordinates.find({ i, j });
			int count = 0;
			if (pos == World.coordinates.end())
			{
				if (Calcs(World.coordinates, i, j, count, false))
				{
					int x = i / CELL_SIZE;
					int y = j / CELL_SIZE;

					World.field[x * HIGHT / CELL_SIZE + y].isAlive = true;
					World.coordinates.insert({ i, j });
				}
			}
			else
			{
				if (Calcs(World.coordinates, i, j, count, true) == false)
				{
					int x = i / CELL_SIZE;
					int y = j / CELL_SIZE;
					World.field[x * HIGHT / CELL_SIZE + y].isAlive = false;
					World.coordinates.erase({ i, j });
				}
			}
		}
	}
	int i = 0;
	for (auto it : World.field)
	{
		arrays->cells[i] = it.isAlive;
		i++;
	}
	int isContinue = 0;
	MPI_Ssend(arrays->cells, (int)World.field.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Recv(&isContinue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return isContinue;
}

bool Calcs(std::set<std::pair<int, int>>& values, int x, int y, int& count, bool isCurCellAlive)
{
	CalculateNeighbor(values, { x - CELL_SIZE, y - CELL_SIZE }, count, isCurCellAlive);
	CalculateNeighbor(values, { x, y - CELL_SIZE }, count, isCurCellAlive);
	CalculateNeighbor(values, { x + CELL_SIZE, y - CELL_SIZE }, count, isCurCellAlive);
	CalculateNeighbor(values, { x - CELL_SIZE, y }, count, isCurCellAlive);
	CalculateNeighbor(values, { x + CELL_SIZE, y }, count, isCurCellAlive);
	CalculateNeighbor(values, { x - CELL_SIZE, y + CELL_SIZE }, count, isCurCellAlive);
	CalculateNeighbor(values, { x, y + CELL_SIZE }, count, isCurCellAlive);
	CalculateNeighbor(values, { x + CELL_SIZE, y + CELL_SIZE }, count, isCurCellAlive);

	if (isCurCellAlive == false && count == 3)
		return true;
	else if (isCurCellAlive == true && (count == 3 || count == 2))
		return true;
	return false;
}

void CalculateNeighbor(std::set<std::pair<int, int>>& values, std::pair<int, int> coord, int& count, bool isCurCellAlive)
{
	auto pos = values.find(coord);
	if (pos != values.end())
		count++;
}