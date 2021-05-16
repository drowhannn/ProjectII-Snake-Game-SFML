#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");
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



		simulateGame(&window);

		window.display();

	}

}