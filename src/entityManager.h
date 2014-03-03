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

	void update();
	
	bool gameover() const;

	std::vector<Species> getSpecies() const { return species; }
	std::vector<Fruit*> getFruits() const { return fruits; }
	
  private:
	typedef struct Position {
		float dist;
		Vect2i pos;

		Position(float _dist = 0) : dist(_dist) {}
	} Position;
	
	std::vector<Fruit*> fruits;
	std::vector<Species> species;
	std::vector<Bush> bushes;
	
	const unsigned int distanceSigmoid;
	const int worldSize, bushesNumber, bushesMinSize, bushesMaxSize, combatDeviation, allowFriendlyFire, randomInCombats;
	
	void update(Animal *animal, const unsigned int index);
	
	void addClosestEnemy(Animal *animal, const unsigned int index, std::vector<float> &inputs);
	void addClosestAlly(Animal *animal, const unsigned int index, std::vector<float> &inputs);
	
	void addClosestFruit(Animal* animal, std::vector<float> &inputs);
	
	Animal* getClosestAnimalFromTab(const Animal *animal, const std::vector<Animal*> &tab, Position &closestPos, bool animalInTab);
	
	Vect2i wrapPositionDifference(const Vect2i a, const Vect2i b) const;
	
	void addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle);
	
	void handleCollisionsWithPointers(Animal *animal);
	void handleCollisions(Animal *animal, const unsigned int index);
	// returns true if animal died
	bool battle(Animal *animal, Animal *enemy);
	
	bool isColliding(const Entity *a, const Entity *b) const;
	
};

#endif // ENTITY_MANAGER_H
