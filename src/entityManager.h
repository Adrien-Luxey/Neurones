#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include "utils.h"

typedef struct Species {
	std::vector<Entity*> tab;

	std::vector<int> prays, predators;
} Species;

class EntityManager {
  public:
	EntityManager();
	~EntityManager();

	void update(const float dt);

	const std::vector<Species> getEntities() { return entities; }
	const int getFruitsIndex() { return fruitsIndex; }
	const int getAnimalsIndex() { return animalsIndex; }
	const int getSpeciesNumber() { return speciesNumber; }

  private:
	std::vector<Species> entities;
	int fruitsIndex, animalsIndex;
	const int speciesNumber, praysNumber, distanceSigmoid, hitbox;

	void update(Animal *animal, const int index, const float dt);
	
	void addClosest(const Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs, const bool isAnimal);
	
	void getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest, const bool isAnimal);
	
	const Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b);
	
	const void addNormalizedPosition(const Position p, std::vector<float> &inputs);
	
	void collisionCheck(Animal *animal, const int index);
	
	const bool isColliding(const Vect2i a, const Vect2i b);
	
	typedef struct Position {
		int dist;
		Vect2i pos;

		Position() : dist(worldSize * worldSize) {}
	} Position;
};

#endif // ENTITY_MANAGER_H
