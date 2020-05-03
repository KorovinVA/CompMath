#include <SFML/Graphics.hpp>
#include <mpi.h>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>

#include <string> 
#include <utility>
#include <list>
#include <set>

//#include "Cell.h"

constexpr auto LENGTH = 1024;
constexpr auto HIGHT = 768;
constexpr int CELL_SIZE = 4;

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
	std::vector<Cell> field;
	std::set<std::pair<int, int>> coordinates;
};

bool operator==(const Universe::Cell & lhs, const Universe::Cell & rhs)
{
	if ((rhs.x == lhs.x) && (lhs.y == rhs.y))
		return true;
	return false;
}

sf::Vector2f calcPos(int i)
{
	i *= CELL_SIZE;
	int x = i / (HIGHT);
	x *= CELL_SIZE;
	int y = i;
	y %= HIGHT;
	return sf::Vector2f(x, y);
}

void CalculateNeighbor(std::set<std::pair<int, int>>& values, std::pair<int, int> coord, int& count, bool isCurCellAlive)
{
	auto pos = values.find(coord);
	if (pos != values.end())
		count++;
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

void CreateWorld(Universe& World)
{
	srand((unsigned)time(0));
	for (int i = 0; i < LENGTH; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			int rand = std::rand() % 10;
			if (rand > 8)
			{
				World.field.push_back(Universe::Cell(i, j, true));
				World.coordinates.insert({ i, j });
			}
		}
	}
	for (auto it : World.field)
	{
		if (it.isAlive)
		{
			//std::cout << it.x << " " << it.y << std::endl;
		}
	}
}

bool CreateNextGen(Universe& World, int Part, int Psize)
{
	int* alives = (int*)calloc((Part / CELL_SIZE) * (HIGHT / CELL_SIZE), sizeof(int));
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
		alives[i] = it.isAlive;
		i++;
	}
	int isContinue = 0;
	MPI_Send(alives, (Part / CELL_SIZE) * (HIGHT / CELL_SIZE), MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Recv(&isContinue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	free(alives);
	return isContinue;
}

void CreateBoundaryArrays()
{
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
			MyWorld.coordinates.insert({x, y});
		}
	}
	for (auto it : MyWorld.field)
	{
		//if(it.isAlive) std::cout << it.x << " " << it.y << std::endl;
	}
	CreateBoundaryArrays();
	int isContinue = 0;
	do {
		isContinue = CreateNextGen(MyWorld, Part, Psize);
	} while (isContinue != 0);
}

void DoRootWork(int Psize)
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Life");

	Universe world;
	CreateWorld(world);

	sf::Text fps;
	sf::Font textFont;
	textFont.loadFromFile("Montserrat-Regular.ttf");
	fps.setFont(textFont);
	fps.setCharacterSize(20);
	fps.setFillColor(sf::Color::White);
	fps.setPosition(LENGTH - 100, 0);

	sf::Clock clock;
	clock.restart();

	std::pair<int, int> end = { -1, -1 };
	int arrSize = (int(LENGTH / (Psize - 1) / CELL_SIZE) * (HIGHT / CELL_SIZE));
	int* alives = (int*)calloc(arrSize, sizeof(int));
	for (int i = 1; i < Psize; ++i)
	{
		for (auto it : world.field)
		{
			MPI_Ssend(&it.x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Ssend(&it.y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		MPI_Ssend(&end.first, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Ssend(&end.second, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	world.field.clear();
	while (window.isOpen())
	{
		window.clear(sf::Color::Black);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		int Framerate = int(1.f / clock.getElapsedTime().asSeconds());
		fps.setString("FPS: " + std::to_string(Framerate));
		clock.restart();

		int contin = 1;
		for (int i = 1; i < Psize; ++i)
		{
			MPI_Recv(alives, arrSize, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&contin, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < arrSize; ++i)
		{
			//std::cout << alives[i] << std::endl;
		}

		for (int i = 0; i < arrSize; ++i)
		{
			if (alives[i] != 0)
			{
				sf::RectangleShape m_cell(sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));
				m_cell.setFillColor(sf::Color::Yellow);
				m_cell.setPosition(calcPos(i));
				//std::cout << calcPos(i).x << " " << calcPos(i).y << std::endl;
				window.draw(m_cell);
			}
		}
		window.draw(fps);
		window.display();
	}
}

int main(int argc, char* argv[])
{
	int Prank = 0;
	int Psize = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &Prank);
	MPI_Comm_size(MPI_COMM_WORLD, &Psize);

	if (Prank != 0)
		DoNotRootWork(Prank, Psize);
	else
		DoRootWork(Psize);

	MPI_Finalize();
	return 0;
}