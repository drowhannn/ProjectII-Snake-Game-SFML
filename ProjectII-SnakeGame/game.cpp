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
	sf::Mouse mouse;
	sf::Vector2i mousePos = mouse.getPosition(window);

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
		text.setPosition(WIDTH / 2.1f, HEIGHT / 2.5f);
		window.draw(text);//play

		text.setString("HighScores");
		text.setPosition(WIDTH / 2.3f, HEIGHT / 1.9f);
		window.draw(text);//highscores

		text.setString("Exit");
		text.setPosition(WIDTH / 2.1f, HEIGHT / 1.5f);
		window.draw(text);//exit


		//option selector
		if (mainmenuSelection == 0)
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 2.45f);
		}
		else if(mainmenuSelection==1)
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.85f);
		}
		else
		{
			optionSelector.setPosition(WIDTH / 3.f, HEIGHT / 1.45f);
		}
		window.draw(optionSelector);

		//mouse click
		bool isCursorClickable = false;

		if (mousePos.y > 250 && mousePos.y<350) mainmenuSelection = 0;
		else if (mousePos.y > 350 && mousePos.y<450) mainmenuSelection = 1;
		else if (mousePos.y > 450 && mousePos.y<550) mainmenuSelection = 2;

		if (mousePos.x > 530 && mousePos.x < 780)
		{
			isCursorClickable = true;
		}

		//input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)
			|| (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)&& isCursorClickable))
		{
			switch (mainmenuSelection)
			{
			case 0:
				newGame = true;
				currentState = GAMEPLAY;
				break;
			case 1:
				currentState = HIGHSCORE;
				break;
			case 2:
				exit(0);
				break;
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

		//mouse
		bool isCursorClickable = false;

		if (mousePos.x > 390 && mousePos.x < 890 &&
			mousePos.y>350 && mousePos.y < 470)
		{
			isCursorClickable = true;
		}


		//input

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) ||
			(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isCursorClickable))
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

		for (int i = 0; i < 5; i++)
		{
			text.setPosition(WIDTH / 2.f - 30, HEIGHT / 2.f - 175 + i * 75);
			str = std::to_string(i + 1);
			text.setString(str);
			window.draw(text);
			text.setString(".");
			text.setPosition(WIDTH / 2.f - 13, HEIGHT / 2.f - 175 + i * 75);
			window.draw(text);

		}
		
		text.setCharacterSize(40);
		text.setFont(scoreFont);
		for (int i = 0; i < 5; i++)
		{
			text.setPosition(WIDTH / 2.f+10, (HEIGHT / 2.f - 175) + i * 75);
			str = std::to_string(gamedata.getHighScore(i));
			text.setString(str);
			window.draw(text);

		}

		text.setString("BACK TO MAINMENU");
		text.setFont(optionsFont);
		text.setCharacterSize(30);
		text.setPosition(WIDTH / 2.f - 140, HEIGHT / 2.f + 200);
		window.draw(text);

		//mouse
		bool isCursorClickable = false;

		if (mousePos.x > 475 &&
			mousePos.x < 830 &&
			mousePos.y >540 &&
			mousePos.y < 615)
		{
			isCursorClickable = true;
		}
		//input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) ||
			(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isCursorClickable))
		{
			currentState = MAINMENU;
		}


	}

}
