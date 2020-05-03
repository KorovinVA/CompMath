#pragma once
#include <SFML/Graphics.hpp>

class Master
{
	int m_size;
	int m_part;
	int transmittSize;
	int LastProctransmittSize;
	std::vector<Universe::Cell> Init;

	sf::RenderWindow m_window;
	sf::Clock m_clock;
	int** cells;
	sf::Text fps;
	sf::Font textFont;

	sf::Vector2f calcPosition(int pos);
	void CreateWorld();
	void LoopProcess();
public:
	Master(int Psize);
	void run();
	~Master();
};