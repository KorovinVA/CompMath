#include <SFML/Graphics.hpp>
#include <mpi.h>

#include <ctime>
#include <iostream>
#include <cstdlib>

#include <string> 

#include "Universe.h"

void DoRootWork(int Psize);
sf::Vector2f calcPosition(int i);
void CreateWorld(std::vector<Universe::Cell>& Init);

void DoRootWork(int Psize)
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Life");

	std::vector<Universe::Cell> Init;
	CreateWorld(Init);

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
	int Part = LENGTH / (Psize - 1);
	int LastProcPart = Part + LENGTH % (Psize - 1);
	int LastProcSize = 0;
	int arrSize = 0;
	for (int i = 0; i < Part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
			arrSize++;
	}
	for (int i = 0; i < LastProcPart; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
			LastProcSize++;
	}
	int** alives = (int**)calloc(Psize - 1, sizeof(int*));
	//std::cerr << arrSize << " : " << LastProcSize << std::endl;
	for (int i = 0; i < Psize - 1; i++)
	{
		if (i == Psize - 2)
			alives[i] = (int*)calloc(LastProcSize, sizeof(int));
		else
			alives[i] = (int*)calloc(arrSize, sizeof(int));
	}
	for (auto it : Init)
	{
		int Dest = it.x / Part + 1;
		it.x = it.x - (Dest - 1)* Part;
		it.x = it.x - it.x % 4;
		MPI_Ssend(&it.x, 1, MPI_INT, Dest, 0, MPI_COMM_WORLD);
		MPI_Ssend(&it.y, 1, MPI_INT, Dest, 0, MPI_COMM_WORLD);
	}
	for (int i = 1; i < Psize; ++i)
	{
		MPI_Ssend(&end.first, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Ssend(&end.second, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	Init.clear();

	while (window.isOpen())
	{
		window.clear(sf::Color::Black);

		int proceed = 1;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				proceed = 0;
		}

		int Framerate = int(1.f / clock.getElapsedTime().asSeconds());
		fps.setString("FPS: " + std::to_string(Framerate));
		clock.restart();

		for (int i = 1; i < Psize; ++i)
		{
			int size = arrSize;
			if (i == Psize - 1)
				size = LastProcSize;
			//std::cerr << "Master:" << size << std::endl;
			MPI_Recv(alives[i - 1], size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&proceed, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			//if(i == 3) std::cerr << 1;
		}

		for (int i = 0; i < Psize - 1; ++i)
		{
			int size = arrSize;
			if (i == Psize - 2)
				size = LastProcSize;
			for (int j = 0; j < size; j++)
			{
				if (alives[i][j] != 0)
				{
					sf::RectangleShape m_cell(sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));
					m_cell.setFillColor(sf::Color::Yellow);
					sf::Vector2f LocalPosition = calcPosition(j);
					sf::Vector2f GlobalPosition(LocalPosition.x + i * LENGTH / (Psize - 1), LocalPosition.y);
					m_cell.setPosition(GlobalPosition);
					window.draw(m_cell);
				}
			}
		}
		window.draw(fps);
		window.display();

		if (proceed == 0)
		{
			window.close();
		}
	}

	for (int i = 0; i < Psize - 1; i++)
	{
		free(alives[i]);
	}
	free(alives);
}

sf::Vector2f calcPosition(int i)
{
	i *= CELL_SIZE;
	int x = i / (HIGHT);
	x *= CELL_SIZE;
	int y = i % HIGHT;
	return sf::Vector2f(float(x), float(y));
}


void CreateWorld(std::vector<Universe::Cell>& Init)
{
	srand((unsigned)time(0));
	for (int i = 0; i < LENGTH; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			int rand = std::rand() % 10;
			if (rand > 8)
			{
				Init.push_back(Universe::Cell(i, j, true));
			}
		}
	}
}