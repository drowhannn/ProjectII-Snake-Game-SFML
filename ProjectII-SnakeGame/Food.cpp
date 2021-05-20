#include "Food.hpp"
#include"game.hpp"

Food::Food()
{
	food.setSize(sf::Vector2f(foodSize, foodSize));
	food.setOrigin(sf::Vector2f(foodSize / 2.f, foodSize / 2.f));
	foodTexture.loadFromFile(FOOD_TEXTURE);
	food.setTexture(&foodTexture);
}

void Food::setDefault()
{
	foodAllocated = false;
}

void Food::display(sf::RenderWindow* window)
{
	if (!foodAllocated)
	{
		srand(time(0));
		foodX = rand() % (int)(WIDTH - BORDERWIDTH - foodSize / 2.f);
		foodY = rand() % (int)(HEIGHT - BORDERHEIGHT - foodSize / 2.f);

		if (foodX < BORDERWIDTH + foodSize) foodX = BORDERWIDTH + foodSize;
		if (foodY < BORDERHEIGHT + foodSize) foodY = BORDERHEIGHT + foodSize;

		foodAllocated = true;
	}

	food.setPosition(sf::Vector2f(foodX, foodY));
	window->draw(food);
}