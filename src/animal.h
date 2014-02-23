#ifndef ANIMAL_H
#define ANIMAL_H

#include <vector>
#include <cmath>

#include "entity.h"
#include "neural_network.h"

class Animal : public Entity {
  public :
	Animal();
	virtual ~Animal() {}

	virtual void init();
	void init(const std::vector<float> &DNA);

	void update(const std::vector<float> inputs);
	
	void incrementScore();
	
	void die();

	// getters
	bool isAlive() const { return alive; }
	int getScore() const { return score; }
	int getClosestEnemyAngle() const { return closestEnemyAngle; }
	int getClosestFruitAngle() const { return closestFruitAngle; }
	int getClosestAllyAngle() const { return closestAllyAngle; }
	float getAttackRate() const { return attackRate; }
	float getDefenseRate() const { return defenseRate; }
	std::vector<float> getDNA();

  protected :		
	int score, closestEnemyAngle, closestFruitAngle, closestAllyAngle;
	float attackRate, defenseRate; // attack/defensekRate : [0; 1]
	bool alive;
	
	const int animalLinearSpeed, animalAngularSpeed;

	NeuralNetwork network;

	// Convertit le déplacement des roues gauche/droite pour modifier la position et l'angle
	void updatePosition(float da, float dp);
};

#endif // ANIMAL_H
