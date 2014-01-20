#include "animal.h"

#define SEUIL_ATTACKING	0.9f

Animal::Animal()
: Entity(), animalSpeed(CFG->readInt("AnimalSpeed")), animalLife(CFG->readInt("AnimalLife")),
	thresholdForTrue(CFG->readFloat("ThresholdForTrue")), network(NETWORK_INPUTS, NETWORK_OUTPUTS) {
	 size = Vect2i(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight"));
	 init();
}

void Animal::init() {
	attackRate = 0.f;
	defenseRate = 0.f;
	score = 0;
	life = animalLife;
	closestPrayAngle = 0;
	closestPredatorAngle = 0;
	closestFruitAngle = 0;
	
	pos.x = rand() % worldSize;
	pos.y = rand() % worldSize;
	angle = rand() % 360;
}

void Animal::init(const std::vector<float> &DNA) {
	init();
	
	network.setDNA(DNA);
}

void Animal::update(const std::vector<float> inputs, const float dt) {
	std::vector<float> outputs;
	
	// Calcul des closestAngles à partir des inputs
	// ... codé en dur...
	// plus proche fruit
	if (inputs[0] != 0.f)
		closestFruitAngle = inputs[0] * 360 + angle;
	else
		closestFruitAngle = 0.f;
	
	// Plus proche proie
	if (inputs[2] != 0.f)
		closestPrayAngle = inputs[2] * 360 + angle;
	else
		closestPrayAngle = 0.f;
	
	// Plus proche predateur
	if (inputs[4] != 0.f)
		closestPredatorAngle = inputs[4] * 360 + angle;
	else
		closestPredatorAngle = 0.f;
	
	// Récupération des sorties du nn
	outputs = network.run(inputs);
	
	// attackRate [0; 1] alors que ouputs[x] [-1; 1], donc on convertit
	if (outputs[2] > 0) {
		attackRate = (outputs[2] + 1.f) / 2.f;
		defenseRate = 0.f;
	} else {
		attackRate = 0.f;
		defenseRate = (-outputs[2] + 1.f) / 2.f;
	}
	
	// Mise à jour de la position si on est pas en train d'attaquer
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
	// facteur de ralentissement : inversement proportionnel à la moyenne d'attaque et de défense
	//float slowdownRate = 1.f - (attackRate + defenseRate) / 2.f;
	// Une autre solution serait de prendre le maximum et non la moyenne des deux actions
	float slowdownRate = 1.f - ((attackRate > defenseRate) ? attackRate : defenseRate) / 2;
	
	// composante angulaire et linéaire du déplacement
	float dp = (left + right) * animalSpeed * slowdownRate;
	float da = (left - right) * animalSpeed * slowdownRate;

	// Application aux données du mobile
	angle = fmod(angle + da * dt, 360.f);
	pos.x = (int) (pos.x + dp * cosf(angle/180.f*PI) * dt) % worldSize;
	pos.y = (int) (pos.y + dp * sinf(angle/180.f*PI) * dt) % worldSize;
	
	// Le module sort parfois des résultats négatifs, on doit corriger ca :
	if (pos.x < 0)
		pos.x += worldSize;
	if (pos.y < 0)
		pos.y += worldSize;
}

bool Animal::isAlive() const {
	return life > 0.f;
}
bool Animal::isAlive(const float dt) {
	life -= dt;
	return isAlive();
}

std::vector<float> Animal::getDNA() {
	return network.getDNA();
}
