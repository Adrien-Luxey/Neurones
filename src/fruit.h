#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h>

#include "utils.h"
#include "entity.h"

typedef struct Bush {
	Vect2i pos;
	int size;
} Bush;

class Fruit : public Entity {
	public :
		Fruit();
		virtual ~Fruit() {}

		virtual void init();
		void init(const Bush &bush);
};

#endif // FRUIT_H
