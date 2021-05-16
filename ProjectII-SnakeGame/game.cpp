#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

sf::RectangleShape snake[MAX];
sf::RectangleShape food;
sf::RectangleShape gameplayBackground(sf::Vector2f(WIDTH, HEIGHT));
sf::RectangleShape optionSelector(sf::Vector2f(20.0f, 20.0f));
sf::RectangleShape gameoverBackground(sf::Vector2f(500.0f, 300.0f));


sf::Texture gameplayBackgroundTexture;
sf::Texture optionSelectorTexture;
sf::Texture snakeHeadTexture, snakeTail1Texture, snakeTail2Texture;
sf::Texture foodTexture;
sf::Texture gameoverBackgroundTexture;

sf::Text text;

sf::Font titleFont;
sf::Font optionsFont;
sf::Font scoreFont;

int mainmenuSelection;

int lengthSnake;
float snakeX[MAX], snakeY[MAX];
bool foodAllocated;
int score;

float foodX, foodY;

float snakeSize = 20.0f;
float snakeSpeed = 20.0f;
float foodSize = 20.0f;

bool newGame = true;

bool firstLoop = true;

SnakeDirection snakeDirection;

GameState currentState;

std::string str;

class GameData {
public:
	int highScores[5];

	GameData()
	{
		for (int i = 0; i < 5; i++)
		{
			highScores[i] = 0;
		}
	}



};

GameData gamedata;


void updateHighScore(int score)
{
	if (gamedata.highScores[4] < score)
	{
		gamedata.highScores[4] = score;
		for (int i = 0; i < 5; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (gamedata.highScores[i] < gamedata.highScores[j])
				{
					int temp = gamedata.highScores[i];
					gamedata.highScores[i] = gamedata.highScores[j];
					gamedata.highScores[j] = temp;
				}
			}
		}

		std::ofstream ofile("gamedata.bin", std::ios::out | std::ios::binary);
		ofile.write(reinterpret_cast<char*>(&gamedata), sizeof(gamedata));
		ofile.close();

	}
}

void init()
{
	gameplayBackgroundTexture.loadFromFile("Resources/bg3.png");
	gameplayBackground.setTexture(&gameplayBackgroundTexture);

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

	food.setSize(sf::Vector2f(foodSize, foodSize));
	food.setOrigin(sf::Vector2f(foodSize / 2.f, foodSize / 2.f));
	foodTexture.loadFromFile("Resources/food1.png");
	food.setTexture(&foodTexture);

	titleFont.loadFromFile("Fonts/Signatria.ttf");
	optionsFont.loadFromFile("Fonts/Baby Party.ttf");
	scoreFont.loadFromFile("Fonts/SpecialElite-Regular.ttf");

	optionSelectorTexture.loadFromFile("Resources/optionSelector.png");
	optionSelector.setTexture(&optionSelectorTexture);
	optionSelector.setFillColor(sf::Color::Green);

	gameoverBackgroundTexture.loadFromFile("Resources/gameover.png");
	gameoverBackground.setTexture(&gameoverBackgroundTexture);
	gameoverBackground.setOrigin(250.0f, 150.0f);
	gameoverBackground.setPosition(WIDTH / 2.f, HEIGHT / 2.f);

	std::ifstream ifile("gamedata.bin", std::ios::in | std::ios::binary);
	ifile.read(reinterpret_cast<char*>(&gamedata), sizeof(gamedata));
	ifile.close();


}

void setDefault()
{

	lengthSnake = DEFAULTLENGTH;
	for (int i = 0; i < lengthSnake; i++)
	{
		snakeX[i] = WIDTH / 2.f - i * snakeSize;
		snakeY[i] = HEIGHT / 2.f;
	}

	snakeDirection = STOP;
	foodAllocated = false;
	score = 0;

}

void simulateGame(sf::RenderWindow* window)
{
	if (currentState != GAMEOVER) {
		window->clear();
		window->draw(gameplayBackground);
	}
	if (firstLoop)
	{
		init();
		firstLoop = false;
	}


	if (currentState == GAMEPLAY)
	{

		if (newGame)
		{
			setDefault();
			newGame = false;
		}

		//input
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) && snakeDirection != DOWN)
			snakeDirection = UP;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) && snakeDirection != UP)
			snakeDirection = DOWN;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) && snakeDirection != LEFT)
			snakeDirection = RIGHT;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) && snakeDirection != RIGHT && snakeDirection != STOP)
			snakeDirection = LEFT;


		//make tail follow the head
		if (snakeDirection != STOP)
		{
			for (int i = lengthSnake - 1; i > 0; i--)
			{
				snakeX[i] = snakeX[i - 1];
				snakeY[i] = snakeY[i - 1];
			}
		}

		//move the head
		if (snakeDirection == UP) snakeY[0] -= snakeSpeed;
		if (snakeDirection == DOWN) snakeY[0] += snakeSpeed;
		if (snakeDirection == RIGHT) snakeX[0] += snakeSpeed;
		if (snakeDirection == LEFT) snakeX[0] -= snakeSpeed;

		//display the snake
		for (int i = 0; i < lengthSnake; i++)
		{
			//std::cout << "Snake[" << i << "] X:" << snakeX[i] << "Y: " << snakeY[i] << std::endl;
			snake[i].setPosition(snakeX[i], snakeY[i]);
			window->draw(snake[i]);
		}

		//food
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


		//Collision
		{
			//collision with walls
			if (snakeX[0] + snakeSize / 2.f > WIDTH - BORDERWIDTH ||
				snakeX[0] - snakeSize / 2.f < BORDERWIDTH ||
				snakeY[0] - snakeSize / 2.f < BORDERHEIGHT ||
				snakeY[0] + snakeSize / 2.f > HEIGHT - BORDERHEIGHT)
			{
				updateHighScore(score);
				currentState = GAMEOVER;
				snakeDirection = STOP;
			}

			//collision with food
			if (snakeX[0] - snakeSize / 2.f < foodX + foodSize / 2.f &&
				snakeX[0] + snakeSize / 2.f > foodX - foodSize / 2.f &&
				snakeY[0] - snakeSize / 2.f < foodY + foodSize / 2.f &&
				snakeY[0] + snakeSize / 2.f > foodY - foodSize / 2.f)
			{
				foodAllocated = false;
				score++;
				lengthSnake++;
			}

			//collision with tail
			for (int i = 1; i < lengthSnake; i++)
			{
				if (snakeX[0] == snakeX[i] &&
					snakeY[0] == snakeY[i])
				{
					updateHighScore(score);
					currentState = GAMEOVER;
					snakeDirection == STOP;
				}
			}


		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			std::cout << "X: " << (float)mousePos.x << "Y: " << (float)mousePos.y << std::endl;
		}

		//display score
		str = std::to_string(score);
		text.setFont(scoreFont);
		text.setPosition(WIDTH / 2.f, BORDERHEIGHT * 2);
		text.setCharacterSize(50);
		text.setString(str);
		window->draw(text);



	}

	else if (currentState == MAINMENU)
	{
		//text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f));

		text.setFont(titleFont);
		text.setString("Snake Game");
		text.setFillColor(sf::Color::Green);
		text.setPosition(WIDTH / 2.75f, HEIGHT / 8.f);
		text.setCharacterSize(100);
		text.setStyle(sf::Text::Bold);

		window->draw(text);//title

		text.setFont(optionsFont);
		text.setFillColor(sf::Color::Cyan);
		text.setCharacterSize(30);

		text.setString("Play");
		text.setPosition(WIDTH / 2.1f, HEIGHT / 2.f);
		window->draw(text);//play

		text.setString("HighScores");
		text.setPosition(WIDTH / 2.3f, HEIGHT / 1.5f);
		window->draw(text);//highscores


		//option selector
		if (mainmenuSelection == 0)
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.9f);
		}
		else
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.45f);
		}
		window->draw(optionSelector);

		//input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if (mainmenuSelection == 0) mainmenuSelection = 1;
			else mainmenuSelection = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			if (mainmenuSelection == 0)
			{
				newGame = true;
				currentState = GAMEPLAY;
			}
			else
			{
				currentState = HIGHSCORE;
			}
		}



	}

	else if (currentState == GAMEOVER)
	{

		window->draw(gameoverBackground);

		text.setString("Final Score: ");
		text.setFont(optionsFont);
		text.setCharacterSize(25);
		text.setPosition(WIDTH / 2.f - 150, HEIGHT / 2.f - 80);
		window->draw(text);

		text.setString(str);
		text.setFont(scoreFont);
		text.setCharacterSize(100);
		text.setPosition(WIDTH / 2.f + 80, HEIGHT / 2.f - 130);
		window->draw(text);


		text.setString("BACK TO MAINMENU");
		text.setFont(optionsFont);
		text.setCharacterSize(40);
		text.setPosition(WIDTH / 2.f - 200, HEIGHT / 2.f + 20);
		window->draw(text);

		//input

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			currentState = MAINMENU;
		}


	}

	else  if (currentState == HIGHSCORE)
	{
		text.setFont(titleFont);
		text.setString("HighScores");
		text.setFillColor(sf::Color::Green);
		text.setPosition(WIDTH / 2.f - 100, BORDERWIDTH + 10);
		text.setCharacterSize(75);
		text.setStyle(sf::Text::Bold);
		window->draw(text);//title

		text.setFont(optionsFont);
		text.setFillColor(sf::Color::Cyan);
		text.setCharacterSize(25);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 200);
		text.setString("1.");
		window->draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 100);
		text.setString("2.");
		window->draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 0);
		text.setString("3.");
		window->draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f + 100);
		text.setString("4.");
		window->draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f + 200);
		text.setString("5.");
		window->draw(text);

		text.setCharacterSize(50);
		text.setFont(scoreFont);
		for (int i = 0; i < 5; i++)
		{
			text.setPosition(WIDTH / 2.f, (HEIGHT / 2.f - 200) + i * 100);
			str = std::to_string(gamedata.highScores[i]);
			text.setString(str);
			window->draw(text);

		}


		//input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			currentState = MAINMENU;
		}


	}

}