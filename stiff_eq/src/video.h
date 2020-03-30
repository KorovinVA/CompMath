#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>

#define ORIGIN (sf::Vector2f(50.f, (float)part * 220.f + 120.f))

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

class CoordinateSystem : public Entity
{
public:
	CoordinateSystem(sf::String OxName, sf::String OyName, ScreenPart part, Resources &res, float IntegrationSegment) :
		m_Ox(sf::Vector2f(1000.f, 2.f)),
		m_Oy(sf::Vector2f(200.f, 2.f)),
		m_OxName(OxName, res.textFont, 15),
		m_OyName(OyName, res.textFont, 15)
	{
		m_Ox.setPosition(ORIGIN);
		m_Ox.setFillColor(sf::Color::White);

		m_Oy.setPosition(m_Ox.getPosition() + sf::Vector2f(0.f, -100.f));
		m_Oy.rotate(90);
		m_Oy.setFillColor(sf::Color::White);

		m_OxName.setPosition(m_Ox.getPosition() + sf::Vector2f(1000.f, 0.f));
		m_OxName.setFillColor(sf::Color::Yellow);

		m_OyName.setPosition(m_Oy.getPosition() + sf::Vector2f(-20.f, 0));
		m_OyName.setFillColor(sf::Color::Yellow);

		char t[10] = { 0 };
		sprintf_s(t, 10, "%.2f\n", IntegrationSegment);
		IntSegment.setString(sf::String(t));
		IntSegment.setFont(res.textFont);
		IntSegment.setCharacterSize(15);
		IntSegment.setPosition(m_Ox.getPosition() + sf::Vector2f(1000.f, -30.f));
	}
	void draw(sf::RenderTarget& target) const
	{
		target.draw(m_Ox);
		target.draw(m_Oy);
		target.draw(m_OxName);
		target.draw(m_OyName);
		target.draw(IntSegment);
	}
private:
	sf::RectangleShape m_Ox;
	sf::RectangleShape m_Oy;
	sf::Text m_OxName;
	sf::Text m_OyName;
	sf::Text IntSegment;
};

class Point : public Entity
{
public:
	Point() = delete;
	Point(sf::Vector2f location, ScreenPart part) :
		my_location(location),
		point(1)
	{
		if(part != Second) point.setPosition(ORIGIN + sf::Vector2f(location.x, -location.y * 100.f));
		else point.setPosition(ORIGIN + sf::Vector2f(location.x, -location.y * 1000000.f));
		point.setFillColor(sf::Color::Yellow);
	}
	void draw(sf::RenderTarget& target) const
	{
		target.draw(point);
	}
	sf::Vector2f getLocation()
	{
		return sf::Vector2f(my_location.x, my_location.y);
	}
private:
	sf::Vector2f my_location;
	sf::CircleShape point;
};

struct PointApply
{
	std::array<Point, 3> PrevPoints;
	bool isapply;
};