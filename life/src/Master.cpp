#include <SFML/Graphics.hpp>
#include <mpi.h>

#include <ctime>
#include <iostream>
#include <cstdlib>

#include <string> 

#include "Universe.h"
#include "Master.h"

sf::Vector2f Master::calcPosition(int pos)
{
	pos *= CELL_SIZE;
	int x = pos / (HIGHT);
	x *= CELL_SIZE;
	int y = pos % HIGHT;
	return sf::Vector2f(float(x), float(y));
}

void Master::CreateWorld()
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

void Master::LoopProcess()
{
	m_window.clear(sf::Color::Black);

	int proceed = 1;
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			proceed = 0;
	}

	int Framerate = int(1.f / m_clock.getElapsedTime().asSeconds());
	fps.setString("FPS: " + std::to_string(Framerate));
	m_clock.restart();

	for (int i = 1; i < m_size; ++i)
	{
		int size = transmittSize;
		if (i == m_size - 1)
			size = LastProctransmittSize;
		MPI_Recv(cells[i - 1], size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Ssend(&proceed, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	for (int i = 0; i < m_size - 1; ++i)
	{
		int size = transmittSize;
		if (i == m_size - 2)
			size = LastProctransmittSize;
		for (int j = 0; j < size; j++)
		{
			if (cells[i][j] != 0)
			{
				sf::RectangleShape m_cell(sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));
				m_cell.setFillColor(sf::Color::Yellow);
				sf::Vector2f LocalPosition = calcPosition(j);
				sf::Vector2f GlobalPosition(LocalPosition.x + i * LENGTH / (m_size - 1), LocalPosition.y);
				m_cell.setPosition(GlobalPosition);
				m_window.draw(m_cell);
			}
		}
	}
	m_window.draw(fps);
	m_window.display();

	if (proceed == 0)
	{
		m_window.close();
	}
}

Master::Master(int Psize) :
	m_window(sf::VideoMode(LENGTH, HIGHT), "Life")
{
	textFont.loadFromFile("Montserrat-Regular.ttf");
	fps.setFont(textFont);
	fps.setCharacterSize(20);
	fps.setFillColor(sf::Color::White);
	fps.setPosition(LENGTH - 100, 0);

	m_size = Psize;
	m_part = LENGTH / (m_size - 1);

	transmittSize = 0;
	LastProctransmittSize = 0;
	for (int i = 0; i < m_part; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
			transmittSize++;
	}
	for (int i = 0; i < m_part + LENGTH % (m_size - 1); i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
			LastProctransmittSize++;
	}

	cells = (int**)calloc(m_size - 1, sizeof(int*));
	for (int i = 0; i < m_size - 1; i++)
	{
		if (i == m_size - 2)
			cells[i] = (int*)calloc(LastProctransmittSize, sizeof(int));
		else
			cells[i] = (int*)calloc(transmittSize, sizeof(int));
	}

	CreateWorld();
}

void Master::run()
{
	std::pair<int, int> end = { -1, -1 };
	for (auto it : Init)
	{
		//std::cerr << it.x << std::endl;
		int Dest = it.x / m_part + 1;
		it.x = it.x - (Dest - 1)* m_part;
		it.x = it.x - it.x % CELL_SIZE;
		MPI_Ssend(&it.x, 1, MPI_INT, Dest, 0, MPI_COMM_WORLD);
		MPI_Ssend(&it.y, 1, MPI_INT, Dest, 0, MPI_COMM_WORLD);
	}
	for (int i = 1; i < m_size; ++i)
	{
		MPI_Ssend(&end.first, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Ssend(&end.second, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	Init.clear();

	while (m_window.isOpen())
		LoopProcess();
}

Master::~Master()
{
	for (int i = 0; i < m_size - 1; i++)
	{
		free(cells[i]);
	}
	free(cells);
}
