#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "entity_manager.h"
#include "utils.h"
#include "animal.h"
#include "fruit.h"

class Game;

/**
 * @brief Class handling every display aspect of the game
 * @param _game	Ptr to its parent game object
 */
class Display {
  public:
	Display(Game* _game);

	~Display() {}
	
	/**
	 * @brief Displays all the game elements and the UI
     * @param manager			Reference to the manager containing all entities to be displayed
     * @param _interpolation	Rate describing the interval of time between 2 game updates
     */
	void update(const EntityManager &manager, const float &_interpolation);
	/**
	 * @brief Handles all the user events
     */
	void events();

  private:
	/**
	 * @brief Display the game elements (animals and plants)
     * @param manager	Reference to the manager containing all entities to be displayed
     * @param view		Reference to the current view of the game scene (what will be seen by the user)
     */
	void displayGame(const EntityManager &manager, const sf::View &view);
	/**
	 * @brief Display the UI elements (the text on the upper left)
     * @param manager	Reference to the manager containing all entities to be displayed
     */
	void displayUI(const EntityManager &manager);
	
	/**
	 * @brief Handles camera events and accordingly modifies mainView 
     */
	void cameraEvents();
	
	/**
	 * @brief Draw the fruits inside the view
     * @param fruits	The fruits list
     * @param view		Game View (what will be seen by the user)
     */
	void drawFruits(const std::vector<Fruit*> &fruits, const sf::View &view);
	/**
	 * @brief Draw the animals inside the view
     * @param animals	The animals list
     * @param view		Game View (what will be seen by the user)
     */
	void drawAnimals(const std::vector<Animal*> &animals, const sf::View &view);
	
	void drawGameBorders(const sf::View &view);
	
	/**
	 * @brief Changes the shapes colors according to the species index
     * @param index	The current species index
     */
	void speciesColor(int index);
	
	/**
	 * @brief Utility function to draw an oriented vector
     * @param pos	Origin of the vector
     * @param angle	Angle in degrees
     * @param color	Color
     * @param size	Size of the vector
     */
	void drawVector(const Vect2i &pos, const float &angle, const sf::Color &color, const sf::Vector2f &size);
	
	/**
	 * @brief Tells wether a point is inside the view
     * @param pos	The point's position
     * @param view	The view
     * @return 
     */
	bool isInsideView(const Vect2i &pos, const sf::View &view);
	
	Game *game;
	sf::RenderWindow window;
	sf::CircleShape fruitShape, animalShape;
	sf::ConvexShape animalHeadShape;
	sf::RectangleShape lineShape;
	sf::Text text;
	sf::Font font;
	sf::View mainView;
	
	const int STATUS_BAR_WIDTH, WORLD_SIZE, VIEW_MOVE_DELTA;
	int windowWidth, windowHeight;
	float interpolation;
	bool hasFocus;
};

#endif // DISPLAY_H
