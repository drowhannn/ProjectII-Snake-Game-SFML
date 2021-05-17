#pragma once



#define WIDTH 1280.0f
#define HEIGHT 720.0f

#define BORDERWIDTH 25.0f
#define BORDERHEIGHT 25.0f

#define FPS 10

enum GameState {
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
	HIGHSCORE

};

void simulateGame(sf::RenderWindow*);