#include "animal.h"

#define SEUIL_ATTACKING	0.9f

Animal::Animal()
: Entity(), ANIMAL_LINEAR_SPEED(CFG->readInt("AnimalLinearSpeed")),
  ANIMAL_ANGULAR_SPEED(CFG->readInt("AnimalAngularSpeed")), network(CFG->readInt("InputLayerSize"), CFG->readInt("OutputLayerSize")) {
	radius = CFG->readInt("AnimalRadius");
	init();
}

void Animal::init() {
	battleOutput = 0.f;
	score = 0;
	alive = true;
	closestEnemyAngle = 0;
	closestFruitAngle = 0;
	closestAllyAngle = 0;
	
	pos.x = rand() % WORLD_SIZE;
	pos.y = rand() % WORLD_SIZE;
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
	
	battleOutput = outputs[2];
	
	// Mise à jour de la position si on est pas en train d'attaquer
	updatePosition(outputs[0], outputs[1]);
}

void Animal::incrementScore() {
	score++;
}

void Animal::die() {
	alive = false;
}

float Animal::getAttackValue() const {
	if (battleOutput <= 0.f)
		return 0.f;
	else
		return battleOutput;
}
float Animal::getDefenseValue() const {
	if (battleOutput >= 0.f)
		return 0.f;
	else
		return -battleOutput;
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
	// slowdown factor : when attacking or defending, animals go slower
	float slowdownRate = 1.f - fabs(battleOutput);
	
	// composante angulaire et linéaire du déplacement
	dp *= ANIMAL_LINEAR_SPEED * slowdownRate;
	da *= ANIMAL_ANGULAR_SPEED * slowdownRate;
	
	// mise à jour de lastPos et speed pour l'interpolation (cf le lien dans game.cpp au sujet de la gameloop)
	speed.x = dp * cosf(angle/180.f*PI);
	speed.y = dp * sinf(angle/180.f*PI);
	lastPos = pos;
	
	// Application aux données du mobile
	angle = fmod(angle + da, 360.f);
	pos.x = (int) (pos.x + speed.x) % WORLD_SIZE;
	pos.y = (int) (pos.y + speed.y) % WORLD_SIZE;
	
	// Le modulo sort parfois des résultats négatifs, on doit corriger ca :
	if (pos.x < 0)
		pos.x += WORLD_SIZE;
	if (pos.y < 0)
		pos.y += WORLD_SIZE;
}

std::vector<float> Animal::getDNA() {
	return network.getDNA();
}
