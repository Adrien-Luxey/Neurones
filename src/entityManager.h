#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include "utils.h"
#include "fruit.h"
#include "animal.h"

typedef struct Species {
	std::vector<Entity*> tab;

	std::vector<int> prays, predators;
} Species;

class EntityManager {
  public:
	EntityManager();
	~EntityManager();

	void update(const float dt);

	std::vector<Species> getEntities() const  { return entities; }
	int getFruitsIndex() const { return fruitsIndex; }
	int getAnimalsIndex() const { return animalsIndex; }
	int getSpeciesNumber() const { return speciesNumber; }	
	
  private:
	typedef struct Position {
		int dist;
		Vect2i pos;

		Position() : dist(0) {}
		Position(int _dist) : dist(_dist) {}
	} Position;
	
	std::vector<Species> entities;
	int fruitsIndex, animalsIndex;
	
	const unsigned int speciesNumber;
	const unsigned int praysNumber;
	const unsigned int distanceSigmoid;
	const int hitbox;
	const int worldSize;
	
	void update(Animal *animal, const int index, const float dt);
	
	void addClosest(Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs, const bool isAnimal);
	
	void getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest, const bool isAnimal);
	
	const Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b);
	
	const void addNormalizedPosition(const Position p, std::vector<float> &inputs);
	
	void collisionCheck(Animal *animal, const int index);
	
	const bool isColliding(const Vect2i a, const Vect2i b);
};

#endif // ENTITY_MANAGER_H
