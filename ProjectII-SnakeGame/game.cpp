#include "game.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include "Snake.hpp"
#include "Food.hpp"
#include "GameData.hpp"

std::string str;

GameData gamedata;

Snake s;

Food f;

Game::Game() : currentState(MAINMENU), mainmenuSelection(0), score(0)
{
	window.create(sf::VideoMode(WIDTH, HEIGHT), GAME_TITLE);

	gameBackground.setSize(sf::Vector2f(WIDTH, HEIGHT));
	gameBackgroundTexture.loadFromFile(GAME_BACKGROUND_TEXTURE);
	gameBackground.setTexture(&gameBackgroundTexture);


	titleFont.loadFromFile(TITLE_FONT);
	optionsFont.loadFromFile(OPTIONS_FONT);
	scoreFont.loadFromFile(SCORE_FONT);

	optionSelector.setSize(sf::Vector2f(20.0f, 20.0f));
	optionSelectorTexture.loadFromFile(OPTION_SELECTOR_TEXTURE);
	optionSelector.setTexture(&optionSelectorTexture);
	optionSelector.setFillColor(sf::Color::Green);

	gameoverBackground.setSize(sf::Vector2f(500.0f, 300.0f));
	gameoverBackgroundTexture.loadFromFile(GAMEOVER_BACKGROUND_TEXTURE);
	gameoverBackground.setTexture(&gameoverBackgroundTexture);
	gameoverBackground.setOrigin(250.0f, 150.0f);
	gameoverBackground.setPosition(WIDTH / 2.f, HEIGHT / 2.f);
}

void Game::run()
{
	window.setFramerateLimit(FPS);
	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		simulateGame();

		window.display();

	}
}

void Game::setDefault()
{
	score = 0;
}


void Game::simulateGame()
{
	if (currentState != GAMEOVER) {
		window.clear();
		window.draw(gameBackground);
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
		s.display(&window);

		//food
		f.display(&window);

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

		//display score
		str = std::to_string(score);
		text.setFont(scoreFont);
		text.setPosition(WIDTH / 2.f, BORDERHEIGHT * 2);
		text.setCharacterSize(50);
		text.setString(str);
		window.draw(text);



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

		window.draw(text);//title

		text.setFont(optionsFont);
		text.setFillColor(sf::Color::Cyan);
		text.setCharacterSize(30);

		text.setString("Play");
		text.setPosition(WIDTH / 2.1f, HEIGHT / 2.f);
		window.draw(text);//play

		text.setString("HighScores");
		text.setPosition(WIDTH / 2.3f, HEIGHT / 1.5f);
		window.draw(text);//highscores


		//option selector
		if (mainmenuSelection == 0)
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.9f);
		}
		else
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.45f);
		}
		window.draw(optionSelector);

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

		window.draw(gameoverBackground);

		text.setString("Final Score: ");
		text.setFont(optionsFont);
		text.setCharacterSize(25);
		text.setPosition(WIDTH / 2.f - 150, HEIGHT / 2.f - 80);
		window.draw(text);

		text.setString(str);
		text.setFont(scoreFont);
		text.setCharacterSize(100);
		text.setPosition(WIDTH / 2.f + 80, HEIGHT / 2.f - 130);
		window.draw(text);


		text.setString("BACK TO MAINMENU");
		text.setFont(optionsFont);
		text.setCharacterSize(40);
		text.setPosition(WIDTH / 2.f - 200, HEIGHT / 2.f + 20);
		window.draw(text);

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
		window.draw(text);//title

		text.setFont(optionsFont);
		text.setFillColor(sf::Color::Cyan);
		text.setCharacterSize(25);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 200);
		text.setString("1.");
		window.draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 100);
		text.setString("2.");
		window.draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 0);
		text.setString("3.");
		window.draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f + 100);
		text.setString("4.");
		window.draw(text);

		text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f + 200);
		text.setString("5.");
		window.draw(text);

		text.setCharacterSize(50);
		text.setFont(scoreFont);
		for (int i = 0; i < 5; i++)
		{
			text.setPosition(WIDTH / 2.f, (HEIGHT / 2.f - 200) + i * 100);
			str = std::to_string(gamedata.getHighScore(i));
			text.setString(str);
			window.draw(text);

		}


		//input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			currentState = MAINMENU;
		}


	}

}
