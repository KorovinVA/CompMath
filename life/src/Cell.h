#pragma once
#include <SFML/Graphics.hpp>

constexpr int CELL_SIZE = 4;

class Cell
{
	sf::Vector2u m_coordinate;
	sf::RectangleShape m_cell;
public:
	Cell(sf::Vector2u coordinate) :
		m_cell(sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE))
	{
		m_cell.setPosition((float)coordinate.x, (float)coordinate.y);
		m_cell.setFillColor(sf::Color::Yellow);

		m_coordinate = coordinate;
	}
	void draw(sf::RenderTarget& target) const
	{
		target.draw(m_cell);
	}
	friend bool operator==(const Cell& lhs, const Cell& rhs);
};


bool operator==(const Cell & lhs, const Cell & rhs)
{
	if(lhs.m_coordinate == rhs.m_coordinate)
		return true;
	return false;
}
