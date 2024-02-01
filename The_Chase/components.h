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
	sf::Texture character;

	Cshape() {};
	Cshape(const float radius, const sf::Color& border_color, float thickness)
		:circle(radius)
	{
		circle.setOrigin(radius, radius);
		circle.setOutlineThickness(thickness);
		circle.setOutlineColor(border_color);
	}

	void load_character(std::string character_name) {
		if (!character.loadFromFile("images/" + character_name + ".png"))
		{
			std::cout << "Failure to load" << character_name;
		}
		character.setSmooth(true);
		circle.setTexture(&character);
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
	int health = 0;
	Clife() {}
	Clife(int duration, int created)
		:lifespan(duration),
		frame_created(created)
	{}

	void set_health(int health)
	{
		this->health = health;
	}
};

