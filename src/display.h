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
	sf::View mainView, minimapView;
	
	const int statusBarWidth, worldSize, viewMoveDelta;
	int windowWidth, windowHeight;
	bool showMinimap, hasFocus;
	
	void displayGame(EntityManager &manager, const sf::View &view);
	void displayUI(EntityManager &manager);
	
	void cameraEvents();
	
	void drawFruits(const std::vector<Entity*> &fruits, const sf::View &view);
	void drawAnimals(const std::vector<Animal*> &animals, const sf::View &view);
	void drawGameBorders(const sf::View &view);
	
	void speciesColor(int index);
	
	void drawVector(const Vect2i &pos, const float &angle, const sf::Color &color, const sf::Vector2f &size);
	
	bool isInsideView(const Vect2i &pos, const sf::View &view);
};

#endif // DISPLAY_H
