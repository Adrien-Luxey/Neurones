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

	void update(const std::vector<float> inputs, const float dt);
	
	void incrementScore();
	
	void die();

	// getters
	bool isAlive();
	bool isAlive(const float dt);
	const bool isAttacking() { return attacking; }
	const int getScore() { return score; }
	const int getClosestPrayAngle() { return closestPrayAngle; }
	const int getClosestPredatorAngle() { return closestPredatorAngle; }

  protected :		
	bool attacking;
	int score, closestPrayAngle, closestPredatorAngle;
	float life;

	NeuralNetwork network;

	// Convertit le déplacement des roues gauche/droite pour modifier la position et l'angle
	void updatePosition(const float left, const float right, const float dt);
};

#endif // ANIMAL_H
