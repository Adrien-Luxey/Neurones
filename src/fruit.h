#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h>

#include "entity.h"

class Fruit : public Entity {
	public :
		Fruit();
		virtual ~Fruit() {}

		virtual void init();
};

#endif // FRUIT_H
