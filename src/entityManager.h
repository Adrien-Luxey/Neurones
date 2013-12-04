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

  private:
	std::vector<Species> entities;
	int fruitsIndex, animalsIndex;
	const int speciesNumber, praysNumber, distanceSigmoid, hitbox;

	void update(Animal *animal, const int index, const float dt);
	
	void addClosest(const Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs);
	
	void getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest);
	
	const Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b);
	
	void collisionCheck(Animal *animal, const int index);
	
	const bool isColliding(const Vect2i a, const Vect2i b);
	
	typedef struct Position {
		int dist;
		Vect2i pos;

		Position() : dist(0) {}
		Position(int _dist) : dist(_dist) {}
	} Position;
};

#endif // ENTITY_MANAGER_H
