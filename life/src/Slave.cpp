#include <mpi.h>
#include <iostream>
#include "Slave.h"

#define SLAVE_TAG 42

void Slave::AllocateSideArrays()
{
	if (m_rank != m_size - 1)
	{
		arrays.MyRstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
		arrays.OthRstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
	}
	if (m_rank != 1)
	{
		arrays.MyLstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
		arrays.OthLstr = (int*)calloc(HIGHT / CELL_SIZE, sizeof(int));
	}
}

void Slave::CalculateNeighbor(std::pair<int, int> coord, int & count)
{
	auto pos = m_world.coordinates.find(coord);
	if (pos != m_world.coordinates.end())
		count++;
	else
	{
		if (coord.first < 0 && m_rank != 1)
		{
			if (arrays.OthLstr[coord.second / CELL_SIZE] != 0)
				count++;
		}
		else if (coord.first >= m_part && m_rank != m_size - 1)
		{
			if (arrays.OthRstr[coord.second / CELL_SIZE] != 0)
				count++;
		}
	}
}

void Slave::putInSideArrays()
{
	if (m_rank != m_size - 1)
	{
		int i = HIGHT / CELL_SIZE - 1;
		for (auto rIt = m_world.field.rbegin(); ; ++rIt)
		{
			arrays.MyRstr[i] = rIt->isAlive;
			i--;
			if (i <= 0)
				break;
		}
	}
	if (m_rank != 1)
	{
		int i = 0;
		for (auto it = m_world.field.begin(); ; ++it)
		{
			arrays.MyLstr[i] = it->isAlive;
			i++;
			if (i >= HIGHT / CELL_SIZE)
				break;
		}
	}
}

void Slave::Communicate()
{
	//->
	if (m_rank == 1 && m_rank != m_size - 1)
		MPI_Ssend(arrays.MyRstr, HIGHT / CELL_SIZE, MPI_INT, m_rank + 1, SLAVE_TAG, MPI_COMM_WORLD);
	else if (m_rank != 1 && m_rank != m_size - 1)
	{
		MPI_Recv(arrays.OthLstr, HIGHT / CELL_SIZE, MPI_INT, m_rank - 1, SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Ssend(arrays.MyRstr, HIGHT / CELL_SIZE, MPI_INT, m_rank + 1, SLAVE_TAG, MPI_COMM_WORLD);
	}
	else if(m_rank == m_size - 1 && m_rank != 1)
		MPI_Recv(arrays.OthLstr, HIGHT / CELL_SIZE, MPI_INT, m_rank - 1, SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//<-
	if (m_rank != 1 && m_rank == m_size - 1)
		MPI_Ssend(arrays.MyLstr, HIGHT / CELL_SIZE, MPI_INT, m_rank - 1, SLAVE_TAG, MPI_COMM_WORLD);
	else if (m_rank != 1 && m_rank != m_size - 1)
	{
		MPI_Recv(arrays.OthRstr, HIGHT / CELL_SIZE, MPI_INT, m_rank + 1, SLAVE_TAG , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Ssend(arrays.MyLstr, HIGHT / CELL_SIZE, MPI_INT, m_rank - 1, SLAVE_TAG, MPI_COMM_WORLD);
	}
	else if (m_rank == 1 && m_rank != m_size - 1)
		MPI_Recv(arrays.OthRstr, HIGHT / CELL_SIZE, MPI_INT, m_rank + 1, SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

bool Slave::CreateNextGen()
{
	putInSideArrays();
	Communicate();
	for (int i = 0; i < m_part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			auto pos = m_world.coordinates.find({ i, j });
			int count = 0;
			if (pos == m_world.coordinates.end())
			{
				if (Calcs(i, j, count, false))
				{
					int x = i / CELL_SIZE;
					int y = j / CELL_SIZE;

					m_world.field[x * HIGHT / CELL_SIZE + y].isAlive = 1;
					m_world.coordinates.insert({ i, j });
				}
			}
			else
			{
				if (Calcs(i, j, count, true) == false)
				{
					int x = i / CELL_SIZE;
					int y = j / CELL_SIZE;

					m_world.field[x * HIGHT / CELL_SIZE + y].isAlive = 0;
					m_world.coordinates.erase({ i, j });
				}
			}
		}
	}

	int i = 0;
	for (auto it : m_world.field)
	{
		arrays.cells[i] = it.isAlive;
		i++;
	}

	int isContinue = 0;
	MPI_Ssend(arrays.cells, transmittSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Recv(&isContinue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return isContinue;
}

bool Slave::Calcs(int x, int y, int & count, bool isCurCellAlive)
{
	CalculateNeighbor({ x - CELL_SIZE, y - CELL_SIZE }, count);
	CalculateNeighbor({ x, y - CELL_SIZE }, count);
	CalculateNeighbor({ x + CELL_SIZE, y - CELL_SIZE }, count);
	CalculateNeighbor({ x - CELL_SIZE, y }, count);
	CalculateNeighbor({ x + CELL_SIZE, y }, count);
	CalculateNeighbor({ x - CELL_SIZE, y + CELL_SIZE }, count);
	CalculateNeighbor({ x, y + CELL_SIZE }, count);
	CalculateNeighbor({ x + CELL_SIZE, y + CELL_SIZE }, count);

	if (isCurCellAlive == false && count == 3)
		return true;
	else if (isCurCellAlive == true && (count == 3 || count == 2))
		return true;
	return false;
}

Slave::Slave(int Prank, int Psize)
{

	m_rank = Prank;
	m_size = Psize;
	m_part = int(LENGTH / (m_size - 1));
	if (m_rank == m_size - 1)
		m_part += LENGTH % (m_size - 1);

	int elementsCount = 0;
	for (int i = 0; i < m_part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			Universe::Cell cell(i, j, 0);
			m_world.field.push_back(cell);
			elementsCount++;
		}
	}

	transmittSize = elementsCount;

	arrays.cells = (int*)calloc(elementsCount, sizeof(int));
	arrays.MyLstr = nullptr;
	arrays.MyRstr = nullptr;
	arrays.OthLstr = nullptr;
	arrays.OthRstr = nullptr;
	AllocateSideArrays();
}

void Slave::run()
{
	int x = 0;
	int y = 0;
	while (x != -1 && y != -1)
	{
		MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		auto pos = std::find(m_world.field.begin(), m_world.field.end(), Universe::Cell(x, y, false));
		if (pos != m_world.field.end())
		{
			pos->isAlive = 1;
			m_world.coordinates.insert({ x, y });
		}
	}
	int isContinue = 0;
	do {
		isContinue = CreateNextGen();
	} while (isContinue != 0);
}

Slave::~Slave()
{
	free(arrays.cells);
	if (arrays.MyLstr) free(arrays.MyLstr);
	if (arrays.OthLstr) free(arrays.OthLstr);
	if (arrays.MyRstr) free(arrays.MyRstr);
	if (arrays.OthRstr) free(arrays.OthRstr);
}
