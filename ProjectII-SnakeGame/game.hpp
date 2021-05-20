#pragma once

#include <SFML/Graphics.hpp>
#include "Definations.hpp"


enum GameState {
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
	HIGHSCORE

};

class Game {
private:
	sf::RenderWindow window;

	sf::RectangleShape gameBackground;
	sf::RectangleShape optionSelector;
	sf::RectangleShape gameoverBackground;


	sf::Texture gameBackgroundTexture;
	sf::Texture optionSelectorTexture;
	sf::Texture gameoverBackgroundTexture;

	sf::Text text;

	sf::Font titleFont;
	sf::Font optionsFont;
	sf::Font scoreFont;

	int mainmenuSelection;

	int score;

	bool newGame = true;

	GameState currentState;


public:
	Game();

	void run();

	void simulateGame();

	void setDefault();
};
