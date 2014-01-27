#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include "utils.h"
#include "fruit.h"
#include "animal.h"

typedef struct Species {
	std::vector<Animal*> tab;
	
	unsigned int aliveAnimals;
} Species;

class EntityManager {
  public:
	EntityManager();
	~EntityManager();
	
	void init();

	void update(const float dt);
	
	bool gameover() const;

	std::vector<Species> getSpecies() { return species; }
	std::vector<Entity*> getFruits() { return fruits; }
	
  private:
	typedef struct Position {
		float dist;
		Vect2i pos;

		Position(float _dist = 0) : dist(_dist) {}
	} Position;
	
	std::vector<Entity*> fruits;
	std::vector<Species> species;
	std::vector<Bush> bushes;
	
	const unsigned int distanceSigmoid;
	const int hitbox, worldSize, bushesNumber, bushesMinSize, bushesMaxSize;
	
	void update(Animal *animal, const unsigned int index, const float dt);
	
	void addClosestEnemy(Animal *animal, const unsigned int index, std::vector<float> &inputs);
	
	void addClosestFruit(Animal* animal, std::vector<float> &inputs);
	
	Entity* getClosestEntityFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest);
	Animal* getClosestAnimalFromTab(const Vect2i pos, const std::vector<Animal*> &tab, Position &closest);
	
	const Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b);
	
	const void addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle);
	
	void collisionCheck(Animal *animal, const unsigned int index);
	
	const bool isColliding(const Vect2i a, const Vect2i b);
};

#endif // ENTITY_MANAGER_H
