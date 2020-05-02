#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#include <ctime>
#include <algorithm>

#include <string> 
#include <utility>
#include <list>
#include <set>

#include "Cell.h"

constexpr auto LENGTH = 1024;
constexpr auto HIGHT = 768;

struct Universe
{
	std::list<Cell> field;
	std::set<std::pair<size_t, size_t>> coordinates;
};

void CalculateNeighbor(std::set<std::pair<size_t, size_t>>& values, std::pair<size_t, size_t> coord, int& count, bool isCurCellAlive)
{
	auto pos = values.find(coord);
	if (pos != values.end())
		count++;
}

bool Calcs(std::set<std::pair<size_t, size_t>>& values, int x, int y, int& count, bool isCurCellAlive)
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
				World.field.push_back(Cell(sf::Vector2u(i, j)));
				World.coordinates.insert({ i, j });
			}
		}
	}
}

void CreateNextGen(Universe& World)
{
	for (int i = 0; i < LENGTH; i += CELL_SIZE)
	{
		for (int j = 0; j < HIGHT; j += CELL_SIZE)
		{
			auto pos = World.coordinates.find({ i, j });
			int count = 0;

			if (pos == World.coordinates.end())
			{
				if (Calcs(World.coordinates, i, j, count, false))
				{
					World.field.push_back(Cell(sf::Vector2u(i, j)));
					World.coordinates.insert({i, j});
				}
			}
			else
			{
				if (Calcs(World.coordinates, i, j, count, true) == false)
				{
					auto deadNow = std::find(World.field.begin(), World.field.end(), Cell(sf::Vector2u(i, j)));
					World.field.erase(deadNow);
					World.coordinates.erase({ i, j });
				}
			}
		}
	}
}

int main()
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

		for (auto it : world.field)
		{
			it.draw(window);
		}
		window.draw(fps);
		window.display();

		CreateNextGen(world);
	}

	return 0;
}