#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include "Snake.hpp"
#include "Food.hpp"
#include "GameData.hpp"

sf::RectangleShape gameplayBackground(sf::Vector2f(WIDTH, HEIGHT));
sf::RectangleShape optionSelector(sf::Vector2f(20.0f, 20.0f));
sf::RectangleShape gameoverBackground(sf::Vector2f(500.0f, 300.0f));


sf::Texture gameplayBackgroundTexture;
sf::Texture optionSelectorTexture;
sf::Texture gameoverBackgroundTexture;

sf::Text text;

sf::Font titleFont;
sf::Font optionsFont;
sf::Font scoreFont;

int mainmenuSelection;

int score;

bool newGame = true;

bool firstLoop = true;

GameState currentState;

std::string str;

GameData gamedata;

Snake s;

Food f;

void init()
{
	gameplayBackgroundTexture.loadFromFile("Resources/bg3.png");
	gameplayBackground.setTexture(&gameplayBackgroundTexture);

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


}

void setDefault()
{
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
			s.setDefault();
			f.setDefault();
			newGame = false;
		}

		//input
		s.input();

		//make tail follow the head
		s.follow();

		//move the head
		s.move();

		//display the snake
		s.display(window);

		//food
		f.display(window);

		//Collision
		{
			//collision with walls and tail
			if (s.collisionWithWalls() || s.collisionWithTail())
			{
				gamedata.update(score);
				currentState = GAMEOVER;
			}

			//collision with food
			if (s.collisionWithFood())
			{
				score++;
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
			str = std::to_string(gamedata.getHighScore(i));
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