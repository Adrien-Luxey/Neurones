#include "fruit.h"

Fruit::Fruit()
: Entity(), size(CFG->readInt("FruitSize"), CFG->readInt("FruitSize")) {}

void Fruit::init() {
	pos.x = rand() % CFG->readInt("WorldSize");
	pos.y = rand() % CFG->readInt("WorldSize");
}
