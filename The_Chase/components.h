#pragma once
#include "vector.h"
#include "SFML/Graphics.hpp"
class Ctransform
{
public:
	Vec2 pos = { 0,0 };
	Vec2 velocity = { 0,0 };
	float angle = 0;
	Ctransform(Vec2 p, Vec2 v, float a)
		: pos(p),
		velocity(v),
		angle(a) {}
};

class Cshape
{
public:
	sf::CircleShape circle;
	Cshape() {};
	Cshape(const float radius, const float point, const sf::Color& fill_color, const sf::Color& border_color, float thickness)
		:circle(radius, point)
	{
		circle.setFillColor(fill_color);
		circle.setOrigin(radius, radius);
		circle.setOutlineThickness(thickness);
		circle.setOutlineColor(border_color);
	}
};

class Cinput
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool special_weapon = false;
	Cinput() {}


};

class Ccollision {
public:
	float radius;
	Ccollision(float r)
		:radius(r) {}
};

class Clife
{
public:
	int lifespan = 0;
	int frame_created = 0;
	Clife(int duration, int created)
		:lifespan(duration),
		frame_created(created) {}
};

