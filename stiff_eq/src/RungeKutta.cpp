#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>
#include "video.h"
#define PRECISION 1

PointApply CalculatePoint(std::array<Point, 3> PrevPoints, float IntegrationSegment, float lastX)
{
	float h = 0.0001f;

	if (PrevPoints[0].getLocation().x >= 1000)
	{
		PointApply ret =
		{
			{
				Point(sf::Vector2f(PrevPoints[0].getLocation().x, 0.f), Entity::ScreenPart::First),
				Point(sf::Vector2f(PrevPoints[0].getLocation().x, 0.f), Entity::ScreenPart::Second),
				Point(sf::Vector2f(PrevPoints[0].getLocation().x, 0.f), Entity::ScreenPart::Third)
			},
			false
		};
		return ret;
	}

	float Y1 = 0.f;
	float Y2 = 0.f;
	float Y3 = 0.f;
	float t  = 0.f;

	float prevt  = PrevPoints[0].getLocation().x;
	float PrevY1 = PrevPoints[0].getLocation().y;
	float PrevY2 = PrevPoints[1].getLocation().y;
	float PrevY3 = PrevPoints[2].getLocation().y;

	float LastY1 = 0.f;
	float LastY2 = 0.f;
	float LastY3 = 0.f;
//Prognosis
	t = prevt + h;
	Y1 = PrevY1 + h * (-0.04f * PrevY1 + 10000 * PrevY2 * PrevY3);
	Y2 = PrevY2 + h * (0.04f * PrevY1 - 10000 * PrevY2 * PrevY3 - 3 * 10000000 * PrevY2 * PrevY2);
	Y3 = PrevY3 + h * (3 * 10000000 * PrevY2 * PrevY2);

//Runge-Kutta
	for (int i = 0; i < PRECISION; ++i)
	{
		LastY1 = PrevY1 + h * (-0.04f * PrevY1 + 10000 * PrevY2 * PrevY3) / 2 + h * (-0.04f * Y1 + 10000 * Y2 * Y3) / 2;
		LastY2 = PrevY2 + h * (0.04f * PrevY1 - 10000 * PrevY2 * PrevY3 - 3 * 10000000 * PrevY2 * PrevY2) / 2 + h *
			(0.04f * Y1 - 10000 * Y2 * Y3 - 3 * 10000000 * Y2 * Y2) / 2;
		LastY3 = PrevY3 + h * (3 * 10000000 * PrevY2 * PrevY2) / 2 + h * (3 * 10000000 * Y2 * Y2) / 2;

		Y1 = LastY1;
		Y2 = LastY2;
		Y3 = LastY3;
	}

//Display
	bool isapply = false;
	if (t - lastX > IntegrationSegment / 1000.f)
	{
		isapply = true;
		t = float(int(t) + 1);
	}

	PointApply ret =
	{
		{
			Point(sf::Vector2f(t, LastY1), Entity::ScreenPart::First),
			Point(sf::Vector2f(t, LastY2), Entity::ScreenPart::Second),
			Point(sf::Vector2f(t, LastY3), Entity::ScreenPart::Third)
		},
		isapply
	};
	return ret;
}