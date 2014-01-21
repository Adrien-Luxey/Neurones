#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "entityManager.h"
#include "utils.h"
#include "animal.h"
#include "fruit.h"

class Game;

class Display {
  public:
	Display(Game* _game);

	~Display() {}

	void update(EntityManager &manager);
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
	
	int statusBarWidth;
	
	void speciesColor(int index);
	
	void drawFruits(const std::vector<Entity*> &fruits);
	void drawAnimals(const std::vector<Animal*> &animals);
	
	void drawVector(const Vect2i &pos, const float &angle, const sf::Color &color, const sf::Vector2f &size);
};

#endif // DISPLAY_H
