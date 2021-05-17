#include "Snake.hpp"
#include "Food.hpp"

extern Food f;

Snake::Snake()
{
	snakeHeadTexture.loadFromFile("Resources/snakeHead1.png");
	snakeTail1Texture.loadFromFile("Resources/snakeTail11.png");
	snakeTail2Texture.loadFromFile("Resources/snakeTail21.png");

	for (int i = 0; i < MAX; i++)
	{
		snake[i].setSize(sf::Vector2f(snakeSize, snakeSize));
		snake[i].setOrigin(sf::Vector2f(snakeSize / 2.f, snakeSize / 2.f));
		if (i == 0) {
			snake[i].setTexture(&snakeHeadTexture);
		}
		else if (i % 2 == 1)
		{
			snake[i].setTexture(&snakeTail1Texture);
		}
		else
		{
			snake[i].setTexture(&snakeTail2Texture);
		}
	}
}

void Snake::setDefault()
{
	lengthSnake = DEFAULTLENGTH;
	for (int i = 0; i < lengthSnake; i++)
	{
		snakeX[i] = WIDTH / 2.f - i * snakeSize;
		snakeY[i] = HEIGHT / 2.f;
	}

	snakeDirection = STOP;

}

int Snake::getLength()
{
	return lengthSnake;
}

void Snake::follow()
{
	if (snakeDirection != STOP){
		for (int i = lengthSnake - 1; i > 0; i--)
		{
			snakeX[i] = snakeX[i - 1];
			snakeY[i] = snakeY[i - 1];
		}
	}
}

void Snake::move()
{
	if (snakeDirection == UP) snakeY[0] -= snakeSize;
	else if (snakeDirection == DOWN) snakeY[0] += snakeSize;
	else if (snakeDirection == RIGHT) snakeX[0] += snakeSize;
	else if (snakeDirection == LEFT) snakeX[0] -= snakeSize;
}

void Snake::display(sf::RenderWindow* window)
{
	for (int i = 0; i < lengthSnake; i++)
	{
		snake[i].setPosition(snakeX[i], snakeY[i]);
		window->draw(snake[i]);
	}
}

void Snake::input()
{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) && snakeDirection != DOWN)
		snakeDirection = UP;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) && snakeDirection != UP)
		snakeDirection = DOWN;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) && snakeDirection != LEFT)
		snakeDirection = RIGHT;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) && snakeDirection != RIGHT && snakeDirection != STOP)
		snakeDirection = LEFT;
}

bool Snake::collisionWithWalls()
{
	if (snakeX[0] + snakeSize / 2.f > WIDTH - BORDERWIDTH ||
		snakeX[0] - snakeSize / 2.f < BORDERWIDTH ||
		snakeY[0] - snakeSize / 2.f < BORDERHEIGHT ||
		snakeY[0] + snakeSize / 2.f > HEIGHT - BORDERHEIGHT)
	{
		snakeDirection = STOP;
		return true;
	}
	return false;
}

bool Snake::collisionWithTail()
{
	for (int i = 1; i < lengthSnake; i++)
	{
		if (snakeX[0] == snakeX[i] &&
			snakeY[0] == snakeY[i])
		{
			snakeDirection = STOP;
			return true;
		}
	}
	return false;
}

bool Snake::collisionWithFood()
{
	if (snakeX[0] - snakeSize / 2.f < f.foodX + f.foodSize / 2.f &&
		snakeX[0] + snakeSize / 2.f > f.foodX - f.foodSize / 2.f &&
		snakeY[0] - snakeSize / 2.f < f.foodY + f.foodSize / 2.f &&
		snakeY[0] + snakeSize / 2.f > f.foodY - f.foodSize / 2.f)
	{
		f.foodAllocated = false;
		lengthSnake++;
		return true;
	}
	return false;

}
