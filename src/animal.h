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

	void update(const std::vector<float> inputs, const float dt);
	
	void incrementScore();
	
	void die();

	// getters
	bool isAlive() const;
	bool isAlive(const float dt);
	bool isAttacking() const { return attacking; }
	int getScore() const { return score; }
	int getClosestPrayAngle() const { return closestPrayAngle; }
	int getClosestPredatorAngle() const { return closestPredatorAngle; }
	std::vector<float> getDNA();

  protected :		
	bool attacking;
	int score, closestPrayAngle, closestPredatorAngle;
	float life;
	
	const int animalSpeed;
	const int animalLife;
	const float thresholdForTrue;

	NeuralNetwork network;

	// Convertit le déplacement des roues gauche/droite pour modifier la position et l'angle
	void updatePosition(const float left, const float right, const float dt);
};

#endif // ANIMAL_H
