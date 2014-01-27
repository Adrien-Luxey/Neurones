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

void Fruit::init(const Bush &bush) {
	float angle = (rand() % 360) * PI / 180.f;
	std::normal_distribution<float> normalRandDist(0, bush.size);
	int dist = abs(normalRandDist(generator));
	
	pos.x = bush.pos.x + cosf(angle) * dist;
	pos.y = bush.pos.y + sinf(angle) * dist;
	
	if (pos.x < 0)
		pos.x += worldSize;
	else if (pos.x >= worldSize)
		pos.x -= worldSize;
	if (pos.y < 0)
		pos.y += worldSize;
	else if (pos.y >= worldSize)
		pos.y -= worldSize;
}