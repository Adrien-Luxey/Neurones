#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include "utils.h"
#include "fruit.h"
#include "animal.h"

typedef struct Species {
	std::vector<Entity*> tab;

	std::vector<int> prays, predators;
	
	unsigned int aliveAnimals;
} Species;

class EntityManager {
  public:
	EntityManager();
	~EntityManager();

	void update(const float dt);
	
	bool gameover() const;

	std::vector<Species> getEntities()   { return entities; }
	int getFruitsIndex() const { return fruitsIndex; }
	int getAnimalsIndex() const { return animalsIndex; }
	int getSpeciesNumber() const { return speciesNumber; }	
	
  private:
	typedef struct Position {
		float dist;
		Vect2i pos;

		Position() {}
		Position(float _dist = 0) : dist(_dist) {}
	} Position;
	
	std::vector<Entity*> fruits;
	std::vector<Species> entities;
	int fruitsIndex, animalsIndex;
	
	const int speciesNumber;
	const int praysNumber;
	const unsigned int distanceSigmoid;
	const int hitbox;
	const int worldSize;
	
	void update(Animal *animal, const int index, const float dt);
	
	Entity* addClosest(Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs, const bool isAnimal);
	
	Entity* addClosestFruit(Animal* animal, std::vector<float> &inputs);
	
	Entity* getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest, const bool isAnimal);
	
	const Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b);
	
	const void addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle);
	
	void collisionCheck(Animal *animal, const int index);
	
	const bool isColliding(const Vect2i a, const Vect2i b);
};

#endif // ENTITY_MANAGER_H
