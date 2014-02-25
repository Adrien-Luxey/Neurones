#include "animal.h"

#define SEUIL_ATTACKING	0.9f

Animal::Animal()
: Entity(), animalLinearSpeed(CFG->readInt("AnimalLinearSpeed")),
  animalAngularSpeed(CFG->readInt("AnimalAngularSpeed")), network(NETWORK_INPUTS, NETWORK_OUTPUTS) {
	 size = Vect2i(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight"));
	 init();
}

void Animal::init() {
	attackRate = 0.f;
	defenseRate = 0.f;
	combatOutput = 0.f;
	score = 0;
	alive = true;
	closestEnemyAngle = 0;
	closestFruitAngle = 0;
	closestAllyAngle = 0;
	
	pos.x = rand() % worldSize;
	pos.y = rand() % worldSize;
	angle = rand() % 360;
	
	speed = Vect2i();
}

void Animal::init(const std::vector<float> &DNA) {
	init();
	
	network.setDNA(DNA);
}

void Animal::update(const std::vector<float> inputs) {
	std::vector<float> outputs;
	
	// Calcul des closestAngles à partir des inputs
	// ... codé en dur...
	// plus proche fruit
	if (inputs[0] != 0.f)
		closestFruitAngle = inputs[0] * 360 + angle;
	else
		closestFruitAngle = 0.f;
	
	// Plus proche ennemi
	if (inputs[2] != 0.f)
		closestEnemyAngle = inputs[2] * 360 + angle;
	else
		closestEnemyAngle = 0.f;
	// Plus proche allié
	if (inputs[5] != 0.f)
		closestAllyAngle = inputs[5] * 360 + angle;
	else
		closestAllyAngle = 0.f;
	
	// Récupération des sorties du nn
	outputs = network.run(inputs);
	
	// attackRate [0; 1] alors que ouputs[x] [-1; 1], donc on convertit
	combatOutput = outputs[2];
	if (outputs[2] > 0) {
		attackRate = outputs[2];
		defenseRate = 0.f;
	} else {
		attackRate = 0.f;
		defenseRate = -outputs[2];
	}
	
	// Mise à jour de la position si on est pas en train d'attaquer
	updatePosition(outputs[0], outputs[1]);
}

void Animal::incrementScore() {
	score++;
}

void Animal::die() {
	alive = false;
}

/* Plus d'infos ici : 
 * http://www.koonsolo.com/news/dewitters-gameloop/
 * L'interpolation c'est la fluidité visuelle à bas prix
 */
Vect2i Animal::getDisplayPos(const float &interpolation) const {
	return Vect2i(	lastPos.x + speed.x * interpolation,
					lastPos.y + speed.y * interpolation );
}

void Animal::updatePosition(float da, float dp) {
	// facteur de ralentissement : inversement proportionnel à la moyenne d'attaque et de défense
	//float slowdownRate = 1.f - (attackRate + defenseRate) / 2.f;
	// Une autre solution serait de prendre le maximum et non la moyenne des deux actions
	float slowdownRate = 1.f - ((attackRate > defenseRate) ? attackRate : defenseRate) / 2;
	
	// composante angulaire et linéaire du déplacement
	dp *= animalLinearSpeed * slowdownRate;
	da *= animalAngularSpeed * slowdownRate;
	
	// mise à jour de lastPos et speed pour l'interpolation (cf le lien dans game.cpp au sujet de la gameloop)
	speed.x = dp * cosf(angle/180.f*PI);
	speed.y = dp * sinf(angle/180.f*PI);
	lastPos = pos;
	
	// Application aux données du mobile
	angle = fmod(angle + da, 360.f);
	pos.x = (int) (pos.x + speed.x) % worldSize;
	pos.y = (int) (pos.y + speed.y) % worldSize;
	
	// Le modulo sort parfois des résultats négatifs, on doit corriger ca :
	if (pos.x < 0)
		pos.x += worldSize;
	if (pos.y < 0)
		pos.y += worldSize;
}

std::vector<float> Animal::getDNA() {
	return network.getDNA();
}
