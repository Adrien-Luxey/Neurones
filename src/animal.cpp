#include "animal.h"

#define SEUIL_ATTACKING	0.8f

Animal::Animal()
: Entity(), network(7, 3) {
	 size = Vect2i(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight"));
}

void Animal::init() {
	attacking = false;
	score = 0;
	life = CFG->readInt("AnimalsLife");
	closestPrayAngle = 0;
	closestPredatorAngle = 0;
	
	pos.x = rand() % worldSize;
	pos.y = rand() % worldSize;
	angle = rand() % 360;
}

void Animal::update(const std::vector<float> inputs, const float dt) {
	std::vector<float> outputs;
	
	// Calcul des closestAngles à partir des inputs
	// ... codé en dur...
	closestPrayAngle = inputs[1] * 360;
	closestPredatorAngle = inputs[3] * 360;
	
	// Récupération des sorties du nn
	outputs = network.run(inputs);
	
	// attacking si output > seuil
	attacking = (outputs[2] >= SEUIL_ATTACKING);
	
	// Mise à jour de la position si on est pas en train d'attaquer
	if (!attacking)
		updatePosition(outputs[0], outputs[1], dt);
}

void Animal::incrementScore() {
	score++;
	life++;
}

void Animal::die() {
	life = 0;
}

// TODO : essayer d'autres moyens
// Comme une différence sur la vitesse, stockée en Vect2f
// qui pourrait parler de différence vitesse/angle séparément plutôt que des roues...
// A voir.
void Animal::updatePosition(const float left, const float right, const float dt) {
	int as = CFG->readInt("AnimalSpeed");

	// composante angulaire et linéaire du déplacement
	float dp = (left + right) * as;
	float da = (left - right) * as;

	// Application aux données du mobile
	angle = fmod(angle + da * dt, 360.f);
	pos.x = (int) (pos.x + dp * cosf(angle/180.f*PI) * dt) % worldSize;
	pos.y = (int) (pos.y + dp * sinf(angle/180.f*PI) * dt) % worldSize;
}

bool Animal::isAlive() {
	return life > 0.f;
}
bool Animal::isAlive(const float dt) {
	life -= dt;
	return isAlive();
}