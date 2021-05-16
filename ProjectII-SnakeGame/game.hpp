#pragma once



#define WIDTH 1280.0f
#define HEIGHT 720.0f

#define BORDERWIDTH 25.0f
#define BORDERHEIGHT 25.0f

#define DEFAULTLENGTH 5

#define FPS 10

#define MAX 60

enum SnakeDirection {
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum GameState {
	MAINMENU,
	GAMEPLAY,
	GAMEOVER,
	HIGHSCORE

};

void simulateGame(sf::RenderWindow*);