#include "entity.h"

#include "stats.h"

// distance cartésienne entre deux points
const float distance(const Vect2i a, const Vect2i b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void Animal::update(std::vector<Entity*> entities, const float dt) {
	if (dead)
		return;
	
	life -= dt;
	//score += dt;
	if (life < 0) {
		dead = true;
		return;
	}
	
	// Vérifier si l'on est sur une plante (auquel cas on la mange et on incrémente notre score)
	posCheck(entities);
	
	// Calcul du réseau de neurones, on récupère les outputs
	std::vector<float> outputs = askNetwork(entities);
	
	int dp = 0, da = 0;
	
	if (CFG->readInt("UseNeuronSigmoid")) {
		// Fonctionnement avec entrées réelles ressemblant à un tank (roues gauche/droite)
		dp = outputs[0] * CFG->readInt("LinearSpeed") + outputs[1] * CFG->readInt("LinearSpeed");
		da = outputs[0] * CFG->readInt("LinearSpeed") - outputs[1] * CFG->readInt("AngularSpeed");
	} else {
		// Fonctionnement avec entrées booléennes
		// 1 && 1 => on avance
		if (outputs[0] && outputs[1]) {
			dp = 2 * CFG->readInt("LinearSpeed");
		// 1 && 0, on tourne à gauche
		} else if (outputs[0]) {
			da = 2 * CFG->readInt("AngularSpeed");
		// 0 && 1 : on tourne à droite
		} else if (outputs[1]) {
			da = 2 * -CFG->readInt("AngularSpeed");
		}
	}
	
	// Calcul de la nouvelle position théorique
	angle += da * dt;
	Vect2i newPos;
	newPos.x = pos.x + dp * cosf(angle/180*PI) * dt;
	newPos.y = pos.y + dp * sinf(angle/180*PI) * dt;
	
	// Modification de la nouvelle position réelle en fonction de la config
	// Soit on enroule les bords (si on dépasse le bord droit on se retrouve au bord gauche) (WrapBorders)
	// Soit on bute contre les bords de l'écran
	// WrapBorders semble moins destabilisant pour les petites bêtes (aussi intelligentes que des mouches, donc)
	int ww = CFG->readInt("WindowWidth");
	int wh = CFG->readInt("WindowHeight");
	if (CFG->readInt("WrapBorders")) {
		isColliding = false;
		if (newPos.x < 0) {
			isColliding = true;
			pos.x = ww + newPos.x;
		} else if (newPos.x >= ww) {
			isColliding = true;
			pos.x = newPos.x - ww;
		} else 
			pos.x = newPos.x;
		
		if (newPos.y < 0) {
			isColliding = true;
			pos.y = wh + newPos.y;
		} else if (newPos.y >= wh) {
			isColliding = true;
			pos.y = newPos.y - wh;
		} else 
			pos.y = newPos.y;
	} else {
		pos.x = (newPos.x >= ww) ? ww - 1 : (newPos.x < 0) ? 0 : newPos.x;
		pos.y = (newPos.y >= wh) ? wh - 1 : (newPos.y < 0) ? 0 : newPos.y;
	}
}

// Réinitialisation de l'animal avec son nouvel ADN donné par l'algo génétique
// On le remet aussi à une position aléatoire, histoire de ne pas croire qu'il s'agisse du même individu
// (en effet son cerveau n'est plus du tout le même dans tous les cas)
void Animal::reinit(const std::vector<float> &DNA, const int _type) {
	init();
	angle = rand() % 360;
	life = CFG->readFloat("InitialLife");
	mType = _type;
	score = 0;
	isColliding = false;
	
	network.setDNA(DNA);
}

// Fonction de vérification pour savoir si on a mangé quelqu'un ou si on s'est fait manger
void Animal::posCheck(std::vector<Entity*> entities) {
	int hitbox = CFG->readInt("Hitbox");
	// le tableau entities en rangé : plants puis animals
	// DOnc pas besoin de parcourir tout le tableau
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (((Animal*) entities[i])->isDead())
			continue;
			
		if (((mType == FOX && entities[i]->type() == MONKEY) || 
			(mType == FOX && entities[i]->type() == LYNX) || 
			(mType == CHICKEN && entities[i]->type() == FOX) || 
			(mType == CHICKEN && entities[i]->type() == MONKEY) ||
			(mType == SNAKE && entities[i]->type() == CHICKEN) || 
			(mType == SNAKE && entities[i]->type() == FOX) ||
			(mType == MONKEY && entities[i]->type() == LYNX) || 
			(mType == MONKEY && entities[i]->type() == SNAKE) || 
			(mType == LYNX && entities[i]->type() == CHICKEN) || 
			(mType == LYNX && entities[i]->type() == SNAKE)) &&
			abs(pos.x - entities[i]->getPos().x) < hitbox &&
			abs(pos.y - entities[i]->getPos().y) < hitbox) {
			
			score++;
			life++;
			((Animal*) entities[i])->dead = true;
		}
	}
}

// Fonction calculant les inputs du NN et qui l'execute, renvoie ses outputs
// Toute donnée qu'on envoie au NN doit être comprise entre -1 et 1, si bien qu'on utilise que des vecteurs unitaires et qu'on passe toute donnée scalaire dans une sigmoide
std::vector<float> Animal::askNetwork(std::vector<Entity*> entities) {
	std::vector<float> inputs;
	// animal orientation (unit vector)
	inputs.push_back(cosf(angle/180.f*PI));
	inputs.push_back(sinf(angle/180.f*PI));
	// Rajoutons la position pour voir
	inputs.push_back(pos.x / (float) CFG->readInt("WindowWidth"));
	inputs.push_back(pos.y / (float) CFG->readInt("WindowHeight"));
	
	// unit vector to closest enemy and food
	unsigned int indexClosestFood = 0, indexClosestEnemy = 0;
	bool foundFood = false, foundEnemy = false;
	float distClosestFood = CFG->readInt("WindowWidth") * CFG->readInt("WindowHeight");
	float distClosestEnemy = CFG->readInt("WindowWidth") * CFG->readInt("WindowHeight");
	float dist2;
	// trouver la plante la plus proche
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (((Animal*) entities[i])->isDead())
			continue;
		
		//food
		if ((mType == FOX && entities[i]->type() == MONKEY) || 
			(mType == FOX && entities[i]->type() == LYNX) || 
			(mType == CHICKEN && entities[i]->type() == FOX) || 
			(mType == CHICKEN && entities[i]->type() == MONKEY) ||
			(mType == SNAKE && entities[i]->type() == CHICKEN) || 
			(mType == SNAKE && entities[i]->type() == FOX) ||
			(mType == MONKEY && entities[i]->type() == LYNX) || 
			(mType == MONKEY && entities[i]->type() == SNAKE) || 
			(mType == LYNX && entities[i]->type() == CHICKEN) || 
			(mType == LYNX && entities[i]->type() == SNAKE)) {
			dist2 = distance(entities[i]->getPos(), pos);
			
			foundFood = true;
			
			if (dist2 < distClosestFood) {
				distClosestFood = dist2;
				indexClosestFood = i;
			}
		}
		// enemy
		else if ((mType == FOX && entities[i]->type() == CHICKEN) || 
			(mType == FOX && entities[i]->type() == SNAKE) || 
			(mType == CHICKEN && entities[i]->type() == SNAKE) || 
			(mType == CHICKEN && entities[i]->type() == LYNX) ||
			(mType == SNAKE && entities[i]->type() == LYNX) || 
			(mType == SNAKE && entities[i]->type() == MONKEY) ||
			(mType == MONKEY && entities[i]->type() == CHICKEN) || 
			(mType == MONKEY && entities[i]->type() == FOX) || 
			(mType == LYNX && entities[i]->type() == MONKEY) || 
			(mType == LYNX && entities[i]->type() == FOX)) {
			dist2 = distance(entities[i]->getPos(), pos);
			
			foundEnemy = true;
			
			if (dist2 < distClosestEnemy) {
				distClosestEnemy = dist2;
				indexClosestEnemy = i;
			}
		}
	}
	
	// Si on a plus de bouffe sur le terrain, on met des valeurs particulières
	if (!foundFood) {
		inputs.push_back(0);
		inputs.push_back(0);
		inputs.push_back(1); // 1 représente l'infini avec la sigmoide
		closestFoodAngle = 0;
	} else {
		// Vecteur unitaire vers la plus proche bouffe
		if (distClosestFood != 0.f) {
			inputs.push_back((entities[indexClosestFood]->getPos().x - pos.x) / distClosestFood);
			inputs.push_back((entities[indexClosestFood]->getPos().y - pos.y) / distClosestFood);
		} else {
			inputs.push_back(0);
			inputs.push_back(0);
		}
		// ajout de la distance (sigmoide [0;1[ )
		distClosestFood = 1 / (1 + expf(-distClosestFood/CFG->readInt("DistanceSigmoid"))) * 2.f - 1.f;
		inputs.push_back(distClosestFood);
		
		// Calcul de l'angle pour l'affichage
		closestFoodAngle = atan2f(entities[indexClosestFood]->getPos().y - pos.y, entities[indexClosestFood]->getPos().x - pos.x) / PI * 180.f;
	}
	
	// Si on a plus de bouffe sur le terrain, on met des valeurs particulières
	if (!foundEnemy) {
		inputs.push_back(0);
		inputs.push_back(0);
		inputs.push_back(1); // 1 représente l'infini avec la sigmoide
		closestEnemyAngle = 0;
	} else {
		// Vecteur unitaire vers le plus proche ennemi
		if (distClosestEnemy != 0.f) {
			inputs.push_back((entities[indexClosestEnemy]->getPos().x - pos.x) / distClosestEnemy);
			inputs.push_back((entities[indexClosestEnemy]->getPos().y - pos.y) / distClosestEnemy);
		} else {
			inputs.push_back(0);
			inputs.push_back(0);
		}
		// ajout de la distance (sigmoide [0;1[ )
		distClosestEnemy = 1 / (1 + expf(-distClosestEnemy/CFG->readInt("DistanceSigmoid"))) * 2.f - 1.f;
		inputs.push_back(distClosestEnemy);
		
		closestEnemyAngle = atan2f(entities[indexClosestEnemy]->getPos().y - pos.y, entities[indexClosestEnemy]->getPos().x - pos.x) / PI * 180.f;
	}
	
	// L'animal est-il en collision avec un mur ?
	inputs.push_back(isColliding);
	
	// Calcul et retour des outputs du NN
	return network.run(inputs);
}
