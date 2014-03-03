#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

/**
 * \file entity_manager.h
 * \author Adrien Luxey
 */

#include <vector>

#include "utils.h"
#include "fruit.h"
#include "animal.h"


/// \brief Structure containing a species of animals
typedef struct Species {
	std::vector<Animal*> tab; ///< The animals tab
} Species;

/**
 * \brief Handles the entities, their relations, updates, and all the game mecanics
 */
class EntityManager {
  public:
	EntityManager();
	~EntityManager();
	
	/**
	 * \brief Initialise game elements for a new generation
     */
	void init();
	
	/**
	 * \brief Update the game elements
     */
	void update();

	// getters
	std::vector<Species> getSpecies() const { return species; }
	std::vector<Fruit*> getFruits() const { return fruits; }
	
  private:	
	/**
	 * \brief Update one animal : give him inputs, call the NN, handle collisions
     * @param animal	Pointer to the animal
     * @param index		The animal's species index in the 'species' tab
     */
	void update(Animal *animal, const unsigned int index);
	
	/**
	 * \brief Add the closest animal's enemy information (pos + combatOutput) to the inputs array
     * @param animal	The animal
     * @param index		The animal's species index
     * @param inputs	The inputs array to fill (to be sent to the NN)
     */
	void addClosestEnemy(const Animal *animal, const unsigned int index, std::vector<float> &inputs);
	/**
	 * \brief Add the closest animal's ally information (pos + combatOutput) to the inputs array
     * @param animal	The animal
     * @param index		The animal's species index
     * @param inputs	The inputs array to fill (to be sent to the NN)
     */
	void addClosestAlly(const Animal *animal, const unsigned int index, std::vector<float> &inputs);
	/**
	 * \brief Add the closest animal's fruit information (pos) to the inputs array
     * @param animal	The animal
     * @param inputs	The inputs array to fill (to be sent to the NN)
     */
	void addClosestFruit(const Animal* animal, std::vector<float> &inputs);
	
	/**
	 * \brief Utility function to get the closest animal from 'animal' in tab
     * @param animal		the animal which we want to find the closest other animal
     * @param tab			the tab where to look for animals
     * @param closestPos	Contains a relative position to an animal. We fill it with the closest in tab if closer from the one we had as input
     * @param animalInTab	Tells wether our animal is in the tab
     * @return				A pointer to the closest animal (if we found one)
	 * 
	 * closestPos is in/out. It contains a relative position from 'animal' to another animal that we found to be close to it.
	 * If our tab contains a closer animal than the one represented by closestPos, we fill closestPos with its relative position to animal and return a pointer to this animal.
	 * If not, we don't change closestPos, and return NULL
     */
	Animal* getClosestAnimalFromTab(const Animal *animal, const std::vector<Animal*> &tab, Position &closestPos, bool animalInTab);
	
	/**
	 * \brief Utility function to retrieve a position that is coherent with ur wrapping-edges world
     * @param a	Our origin point
     * @param b	The absolute position of a point
     * @return	A point representing b relatively to a
	 * 
	 * Let's say a is located in (0px, 800px) and our world is 1000px large
	 * if b is (0, 100px), [ab] = (0, -700px), or a is only 300px (800->1000 + 0->100) away from b in our wrapping world
	 * In such a case, our function will return (0, 1100) so that we can calculate distances without trouble
	 * 
	 * (If we didn't do so, an animal oscillating between an edge would see his world change everytime he crosses it
	 * Which is BAAAAD)
     */
	Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b) const;
	
	/**
	 * \brief Add position p to the inputs array in the right coordinates system so that it is understood by the NN
     * @param p			The position of what we want to output
     * @param inputs	The inputs array (for the NN)
     * @param angle		The angle of our mobile, which we need to calculate realtive angle
     */
	void addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle);
	
	/**
	 * \brief Handles the collisions with fruits/animals of 'animal' after he updated his position 
     * @param animal	The current animal
     * @param index		The animal's species index
     */
	void handleCollisions(Animal *animal, const unsigned int index);
	/**
	 * \brief handles a battle between animal and enemy
     * @param animal	The animal we study
     * @param enemy		His opponent
     * @return			True if animal died, false otherwise
     */
	bool battle(Animal *animal, Animal *enemy);
	
	/**
	 * \brief tells wether a and b collide, considreing both are Entities (ie circles), using their radiuses
     */
	bool isColliding(const Entity *a, const Entity *b) const;
	
	
	/// \brief Structure representing a position with its cartesian size
	typedef struct Position {
		float dist; ///< The size of the vector
		Vect2i pos; ///< The vector position

		Position(float _dist = 0) : dist(_dist) {} ///< A constructor, to be able to define the size
	} Position;
	
	std::vector<Fruit*> fruits;
	std::vector<Species> species;
	std::vector<Bush> bushes;
	
	const unsigned int DISTANCE_SIGMOID;
	const int WORLD_SIZE, BUSHES_NUMBER, BUSHES_MIN_SIZE, BUSHES_MAX_SIZE, BATTLE_DEVIATION;
	const bool ALLOW_FRIENDLY_FIRE, RANDOM_IN_BATTLES;
};

#endif // ENTITY_MANAGER_H
