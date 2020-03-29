#include <SFML/Graphics.hpp>
#include <armadillo>
#include <assert.h>
#include <vector>

#define DARK_GRAY sf::Color::Color(65, 65, 65, 255)

class Entity
{
public:
	enum ScreenPart
	{
		First = 0,
		Second,
		Third
	};

	virtual void draw(sf::RenderTarget& target) const
	{};
};

class Resources
{
public:
	sf::Font textFont;
	void load()
	{
		if (!textFont.loadFromFile("../../media/Montserrat-Regular.ttf"))
		{
			exit(1);
		}
	}
};

class CoordinateSystem: public Entity
{
public:
	CoordinateSystem(sf::String OxName, sf::String OyName, ScreenPart part, Resources &res):
		m_Ox(sf::Vector2f(1000.f, 2.f)),
		m_Oy(sf::Vector2f(200.f, 2.f)),
		m_OxName(OxName, res.textFont, 15),
		m_OyName(OyName, res.textFont, 15)
	{
		m_Ox.setPosition(sf::Vector2f(50.f, (float)part * 220.f + 120.f));
		m_Ox.setFillColor(sf::Color::White);

		m_Oy.setPosition(m_Ox.getPosition() + sf::Vector2f(0.f, -100.f));
		m_Oy.rotate(90);
		m_Oy.setFillColor(sf::Color::White);

		m_OxName.setPosition(m_Ox.getPosition() + sf::Vector2f(1000.f, 0.f));
		m_OxName.setFillColor(sf::Color::Yellow);

		m_OyName.setPosition(m_Oy.getPosition() + sf::Vector2f(-20.f, 0));
		m_OyName.setFillColor(sf::Color::Yellow);
	}
	virtual void draw(sf::RenderTarget& target) const
	{
		target.draw(m_Ox);
		target.draw(m_Oy);
		target.draw(m_OxName);
		target.draw(m_OyName);
	}
private:
	sf::RectangleShape m_Ox;
	sf::RectangleShape m_Oy;
	sf::Text m_OxName;
	sf::Text m_OyName;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "stiff_eq");
	Resources res;
	res.load();

	std::vector<std::unique_ptr<Entity>> world;
	std::unique_ptr<CoordinateSystem> system1(new CoordinateSystem("t", "y1", Entity::ScreenPart::First, res));
	std::unique_ptr<CoordinateSystem> system2(new CoordinateSystem("t", "y2", Entity::ScreenPart::Second, res));
	std::unique_ptr<CoordinateSystem> system3(new CoordinateSystem("t", "y3", Entity::ScreenPart::Third, res));
	world.push_back(std::move(system1));
	world.push_back(std::move(system2));
	world.push_back(std::move(system3));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(DARK_GRAY);
		for (auto it = world.begin(); it != world.end(); ++it)
		{
			it->get()->draw(window);
		}
		window.display();
	}

	return 0;
}