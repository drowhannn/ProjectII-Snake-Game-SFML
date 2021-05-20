#pragma once

#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "Definations.hpp"

enum SnakeDirection {
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Snake
{
private:
	sf::RectangleShape snake[MAX];
	sf::Texture snakeHeadTexture,snakeTail1Texture,snakeTail2Texture;
	int lengthSnake;
	float snakeX[MAX], snakeY[MAX];
	float snakeSize = 20.0f;
	SnakeDirection snakeDirection;

public:
	Snake();

	void setDefault();

	int getLength();

	void follow();

	void move();

	void display(sf::RenderWindow*);

	bool collisionWithWalls();

	bool collisionWithTail();

	bool collisionWithFood();

	void input();


};

