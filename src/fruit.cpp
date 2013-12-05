#include "fruit.h"

Fruit::Fruit()
  : Entity() {
	size = Vect2i(CFG->readInt("FruitRadius"), CFG->readInt("FruitRadius"));
	init();
}

void Fruit::init() {
	pos.x = rand() % worldSize;
	pos.y = rand() % worldSize;
}
