#include <SFML/Graphics.hpp>
#include <assert.h>
#include <vector>
#include <array>
#include "video.h"

#define DARK_GRAY sf::Color::Color(65, 65, 65, 255)
constexpr auto SEGMENT = 40.f;

PointApply CalculatePoint(std::array<Point, 3> PrevPoints, float IntegrationSegment, float lastX);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "stiff_eq");

	Resources res;
	res.load();

	std::vector<CoordinateSystem> CoordSystems;
	CoordSystems.push_back(CoordinateSystem("t", "y1", Entity::ScreenPart::First, res, SEGMENT));
	CoordSystems.push_back(CoordinateSystem("t", "y2", Entity::ScreenPart::Second, res, SEGMENT));
	CoordSystems.push_back(CoordinateSystem("t", "y3", Entity::ScreenPart::Third, res, SEGMENT));

	std::vector<std::array<Point, 3>> points;
//Initial conditions
	std::array<Point, 3> point =
	{
		Point(sf::Vector2f(0.f, 1.f), Entity::ScreenPart::First),
		Point(sf::Vector2f(0.f, 0.f), Entity::ScreenPart::Second),
		Point(sf::Vector2f(0.f, 0.f), Entity::ScreenPart::Third)
	};
	points.push_back(point);

	PointApply prevPoint =
	{
		point,
		true
	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(DARK_GRAY);
		for (auto i : CoordSystems)
		{
			i.draw(window);
		}
		for (auto i = points.begin(); i != points.end(); ++i)
		{
			for (auto j = i->begin(); j != i->end(); ++j)
			{
				j->draw(window);
			}
		}
		prevPoint = CalculatePoint(prevPoint.PrevPoints, SEGMENT, points.back()[0].getLocation().x);
		while (prevPoint.isapply == false)
		{
			prevPoint = CalculatePoint(prevPoint.PrevPoints, SEGMENT, points.back()[0].getLocation().x);
		}
		points.push_back(prevPoint.PrevPoints);
		window.display();
	}

	return 0;
}