#include "fruit.h"

Fruit::Fruit()
  : Entity() {
	size = Vect2i(CFG->readInt("FruitSize"), CFG->readInt("FruitSize"));
	init();
}

void Fruit::init() {
	pos.x = rand() % worldSize;
	pos.y = rand() % worldSize;
}
