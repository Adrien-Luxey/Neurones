#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "utils.h"
#include "entity.h"
#include "stats.h"

class Game;

class Display {
	public :
		Display(Game* _game);
		~Display() {}
		
		void update(std::vector<Entity*> entities);
		void events();
		
		float getElapsedTime();
	private :
		Game *game;
		sf::RenderWindow window;
		sf::RectangleShape animal;
		sf::CircleShape plant;
		sf::RectangleShape line;
		sf::Text text;
		sf::Font font;
		sf::Clock clock;
		
};

#endif // DISPLAY_H
