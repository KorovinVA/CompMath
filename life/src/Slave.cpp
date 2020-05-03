#include <mpi.h>
#include <iostream>

#include "Slave.h"

void Slave::AllocateArrays()
{
}

void Slave::CalculateNeighbor(std::pair<int, int> coord, int & count)
{
	auto pos = m_world.coordinates.find(coord);
	if (pos != m_world.coordinates.end())
		count++;
}

bool Slave::CreateNextGen()
{
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

					m_world.field[x * HIGHT / CELL_SIZE + y].isAlive = true;
					m_world.coordinates.insert({ i, j });
				}
			}
			else
			{
				if (Calcs(i, j, count, true) == false)
				{
					int x = i / CELL_SIZE;
					int y = j / CELL_SIZE;

					m_world.field[x * HIGHT / CELL_SIZE + y].isAlive = false;
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
			Universe::Cell cell(i, j, false);
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
			pos->isAlive = true;
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
}
