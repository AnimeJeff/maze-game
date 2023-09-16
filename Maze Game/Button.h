#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Button : public sf::RectangleShape
{
public:
	
	void checkClick(sf::Vector2f mousePos)
	{
		bool clicked = this->getGlobalBounds().contains(mousePos);
		if (clicked)
		{
			std::cout << "clicked" << std::endl;
		}
	}
	
};

