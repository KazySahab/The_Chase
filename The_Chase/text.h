#pragma once
#include "iostream"
#include "vector.h"
#include "SFML/Graphics.hpp"
class Show_text
{
public:
	sf::Text write;
	sf::Font font;
	Show_text(){}
	Show_text(const sf::Vector2f &position)
	{
		if (!font.loadFromFile("font/h_ghost.ttf"))
		{
			std::cout << "Failed to load Font" << std::endl;
		}
		write.setFont(font);
		write.setCharacterSize(24);
		write.setStyle(sf::Text::Bold);
		write.setFillColor(sf::Color::White);
		write.setPosition(position.x, position.y);
	}
	
};

