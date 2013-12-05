#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "entityManager.h"
#include "utils.h"
#include "entity.h"
class Game;

class Display {
  public:
	Display(Game* _game);

	~Display() {}

	void update(const EntityManager &manager);
	void events();

	float getElapsedTime();
  private:
	Game *game;
	sf::RenderWindow window;
	sf::RectangleShape animalShape;
	sf::CircleShape fruitShape;
	sf::RectangleShape lineShape;
	sf::Text text;
	sf::Font font;
	sf::Clock clock;
	
	void speciesColor(int index);
	
	void drawFruits(const std::vector<Entity*> &fruits);
	void drawAnimals(const std::vector<Entity*> &animals);
};

#endif // DISPLAY_H
