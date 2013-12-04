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

		std::vector<float> update(const std::vector<float> inputs, const float dt);
		
                // getters
                bool isAlive();
                bool isAlive(const float dt);
                const bool isAttacking() { return attacking; }
                const int getScore() { return score; }
		
	protected :		
		bool attacking;
		int score;
		float life;

		NeuralNetwork network;

		// Convertit le déplacement des roues gauche/droite pour modifier la position et l'angle
		void updatePosition(const float left, const float right, const float dt);
};

#endif // ANIMAL_H
