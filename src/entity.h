#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"

// Classe abstraite représentant tout type d'entité à afficher
class Entity {
	public :
		Entity();
		virtual ~Entity() {}
		
		virtual void init() = 0;

		// getters
		Vect2i getPos() const { return pos; }
		int getRadius() const { return radius; }
		float getAngle() const { return angle; }
		
	protected :
		Vect2i pos;
		float angle; // [0°; 360°]
		int radius;
		
		const int worldSize;
};


#endif // ENTITY_H
