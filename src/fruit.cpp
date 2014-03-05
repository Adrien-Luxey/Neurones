#include "fruit.h"

Fruit::Fruit()
  : Entity() {
	radius = CFG->readInt("FruitRadius");
	init();
}

void Fruit::init() {
	pos.x = rand() % WORLD_SIZE;
	pos.y = rand() % WORLD_SIZE;
}

void Fruit::init(const Bush &bush) {
	float angle = (rand() % 360) * PI / 180.f;
	std::normal_distribution<float> normalRandDist(0, bush.size);
	int dist = abs(normalRandDist(generator));
	
	pos.x = bush.pos.x + cosf(angle) * dist;
	pos.y = bush.pos.y + sinf(angle) * dist;
	
	if (pos.x < 0)
		pos.x += WORLD_SIZE;
	else if (pos.x >= WORLD_SIZE)
		pos.x -= WORLD_SIZE;
	if (pos.y < 0)
		pos.y += WORLD_SIZE;
	else if (pos.y >= WORLD_SIZE)
		pos.y -= WORLD_SIZE;
}