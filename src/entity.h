#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"

/**
 * @brief Abstract class represtenting any displayable physical object of the game
 */

class Entity {
	public :
		Entity();
		virtual ~Entity() {}
		
		/**
		 * @brief Abstract method for object initialisation
         */
		virtual void init() = 0;

		// getters
		Vect2i getPos() const { return pos; }
		int getRadius() const { return radius; }
		float getAngle() const { return angle; }
		
	protected :
		Vect2i pos;
		float angle; // degrees
		int radius;
		
		const int WORLD_SIZE;
};


#endif // ENTITY_H
