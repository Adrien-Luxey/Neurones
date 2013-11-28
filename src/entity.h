#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iostream>

#include "utils.h"
#include "neural_network.h"

// Classe virtuelle représentant les entités affichables (plantes, animaux)
class Entity {
	public :
		Entity() : mType(ENTITY) {
			init();
		}
		Entity(int _type) : mType(_type) {
			init();
		}
		
		virtual ~Entity() {}
		
		// Initialisation de l'entit : position aléatoire et angle = 0
		void init() {
			pos.x = rand() % CFG->readInt("WindowWidth");
			pos.y = rand() % CFG->readInt("WindowHeight");
			dead = false;
			
			angle = 0;
		}
		
		// Renvoie le type de l'entité
		const int type() { return mType; };
		
		// Met à jour la position de l'entité
		virtual void update(std::vector<Entity*> entities, const float dt) = 0;
		
		// getters
		const Vect2i getPos() { return pos; }
		const float getAngle() { return angle; }
		const bool isDead() { return dead; }
		
	protected :
		Vect2i pos;
		float angle;
		int mType;
		bool dead;
};

// Animal, doit manger les plantes, bien plus complexe !
class Animal : public Entity {
	public :
		Animal(int _type)
		: Entity(_type), network(NETWORK_INPUTS, NETWORK_OUTPUTS) {
			// Angle initial aléatoire
			angle = rand() % 360;
			life = CFG->readFloat("InitialLife");
			closestFoodAngle = 0;
			closestEnemyAngle = 0;
			score = 0;
			isColliding = false;
		}
		virtual ~Animal() {}
		
		virtual void update(std::vector<Entity*> entities, const float dt);
		
		void reinit(const std::vector<float> &DNA, const int _type);
		
		// Getters
		const unsigned int getScore() { return (int) score; }
		const float getLife() { return life; }
		const std::vector<float> getDNA() { return network.getDNA(); }
		const float getClosestFoodAngle() { return closestFoodAngle; }
		const float getClosestEnemyAngle() { return closestEnemyAngle; }
		
	private :
		void posCheck(std::vector<Entity*> entities);
		std::vector<float> askNetwork(const std::vector<Entity*> entities);
		
		// Affublé d'un cerveau
		NeuralNetwork network;
		float closestFoodAngle, closestEnemyAngle, life, score;
		bool isColliding;
};

#endif // ENTITY_H
