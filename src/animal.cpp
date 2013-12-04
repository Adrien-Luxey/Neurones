#include "animal.h"

#define SEUIL_ATTACKING	0.8f

Animal::Animal()
: Entity(), size(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight")) {}

void Animal::init() {
	int ws = CFG->readInt("WorldSize");
	
	attacking = false;
	score = 0;
	life = CFG->readInt("AnimalsLife");
	
	pos.x = rand() % ws;
	pos.y = rand() % ws;
	angle = rand() % 360;
}

std::vector<float> Animal::update(const std::vector<float> inputs, const float dt) {
	std::vector<float> outputs;
	
	// Récupération des sorties du nn
	outputs = network.run(inputs);
	
	// attacking si output > seuil
	attacking = (outputs[2] >= SEUIL_ATTACKING);
	
	// Mise à jour de la position si on est pas en train d'attaquer
	if (!attacking)
		updatePosition(outputs[0], outputs[1], dt);
}

// TODO : essayer d'autres moyens
// Comme une différence sur la vitesse, stockée en Vect2f
// qui pourrait parler de différence vitesse/angle séparément plutôt que des roues...
// A voir.
void Animal::updatePosition(const float left, const float right, const float dt) {
	int ws = CFG->ReadInt("WorldSize");
	int as = CFG->readInt("AnimalSpeed");

	// composante angulaire et linéaire du déplacement
	float dp = (left + right) * as;
	float da = (left - right) * as;

	// Application aux données du mobile
	angle = fmod(angle + da * dt, 360.f);
	pos.x = (pos.x + dp * cosf(angle/180.f*PI) * dt) % ws;
	pos.y = (pos.y + dp * sinf(angle/180.f*PI) * dt) % ws;
}

bool Animal::isAlive() {
	return life >= 0.f;
}
bool Animal::isAlive(const float dt) {
	life -= dt;
	return isAlive();
}