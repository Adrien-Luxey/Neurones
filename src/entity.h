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
		const Vect2i getPos() { return pos; }
		const Vect2i getSize() { return size; }
		const float getAngle() { return angle; }
	protected :
		Vect2i pos, size;
		float angle; // [0°; 360°]
		
		const int worldSize;
};


#endif // ENTITY_H
