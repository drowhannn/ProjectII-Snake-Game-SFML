#pragma once

#include <SFML/Graphics.hpp>

class Food
{
private:
	sf::RectangleShape food;
	sf::Texture foodTexture;
	bool foodAllocated;
	float foodX, foodY;
	float foodSize = 20.0f;

public:
	Food();

	void setDefault();

	void display(sf::RenderWindow*);

	friend class Snake;

};

