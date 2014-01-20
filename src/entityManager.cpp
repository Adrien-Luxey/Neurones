#include "entityManager.h"

// AnimalsNumber = entities[1].size() (idem pour les fruits)


// TODO : la couleuuur des animaux
/* TODO : cette histoire de Species c'est stupide : considérer que tout le monde est un animal 
 *		  alors qu'on s'est emmerdé à faire une classe Entity and co.
 *		  Il faut un vector contenant prays et predators dont les indices correspondent à ceux des animaux.
 */


EntityManager::EntityManager()
  : speciesNumber((int) CFG->readInt("SpeciesNumber")), praysNumber(speciesNumber/2), distanceSigmoid(CFG->readInt("DistanceSigmoid")),
  hitbox(CFG->readInt("Hitbox")), worldSize(CFG->readInt("WorldSize")) {
	
	Species tmp;
	
	// fruits
	fruits.clear();
	for (int j = 0; j < CFG->readInt("FruitsNumber"); j++)
		fruits.push_back(new Fruit());
	
	
	// animaux
	animalsIndex = entities.size();
	for (int i = 0; i < speciesNumber; i++) {
		tmp.tab.clear();
		tmp.prays.clear();
		tmp.predators.clear();
		
		// Ajout des animaux
		for (int j = 0; j < CFG->readInt("AnimalsNumber"); j++)
			tmp.tab.push_back(new Animal());
		
		tmp.aliveAnimals = tmp.tab.size();
		
		// Ajout des index des prédateurs/proies
		// Une espèce mange praysNumber espèces après elle dans le tableau, et se fait manger par praysNumber avant elle
		for (int j = 1; j <= praysNumber; j++) {
			tmp.prays.push_back(animalsIndex + (i + j) % speciesNumber);
			
			// résolution d'un bug lié au mod en c++
			// -1 % 3 = -1, merci la stl
			if (i - j >= 0) {
				tmp.predators.push_back(animalsIndex + i - j);
			} else {
				tmp.predators.push_back(animalsIndex + i - j + speciesNumber);
			}
		}
		
		entities.push_back(tmp);
	}
}

EntityManager::~EntityManager() {
	// Destruction de toutes les entities
	for (unsigned int i = 0; i < entities.size(); i++) {
		for (unsigned int j = 0; j < entities[i].tab.size(); j++)
			delete entities[i].tab[j];
			
		entities[i].tab.clear();
	}
	
	entities.clear();
	
	for (unsigned int i; i < fruits.size(); i++)
		delete fruits[i];
	fruits.clear();
}

void EntityManager::update(const float dt) {
	// Parcours de toutes les espèces
	for (int i = animalsIndex; i < animalsIndex + speciesNumber; i++) {
		entities[i].aliveAnimals = 0;
		
		// Parcours de tous les animaux de l'espece
		for (unsigned int j = 0; j < entities[i].tab.size(); j++) {
			Animal *animal = (Animal*) entities[i].tab[j];
			
			if (!animal->isAlive(dt))
				continue;
			
			entities[i].aliveAnimals++;
			
			update(animal, i, dt);
			
			collisionCheck(animal, i);
		}
	}
}

bool EntityManager::gameover() const {
	unsigned int aliveSpecies = 0;
	
	for (int i = animalsIndex; i < animalsIndex + speciesNumber; i++) {
		if (entities[i].aliveAnimals > 0)
			aliveSpecies++;
	}
	
	return (speciesNumber == 1 && aliveSpecies == 0) ||
		(speciesNumber > 1 && aliveSpecies < 2);
}

void EntityManager::update(Animal *animal, const int index, const float dt) {
	std::vector<float> inputs;
	Position closestFruit(worldSize * worldSize);
	Animal *closest = NULL;
	
	// Plus proche fruit
	getClosestFromTab(animal->getPos(), entities[ fruitsIndex ].tab, closestFruit, false);
	addNormalizedPosition(closestFruit, inputs, animal->getAngle());
	
	// Plus proche proie
	closest = (Animal*) addClosest(animal, entities[index].prays, inputs, true);
	// Ajout de la valeur att/def de l'animal en question aux inputs
	if (closest != NULL)
		inputs.push_back(closest->getCombatOutput());
	else
		inputs.push_back(0.f);
	
	// Plus proche prédateur
	closest = NULL;
	closest = (Animal*) addClosest(animal, entities[index].predators, inputs, true);
	// Ajout de la valeur att/def de l'animal en question aux inputs
	if (closest != NULL)
		inputs.push_back(closest->getCombatOutput());
	else
		inputs.push_back(0.f);
	
	// Plus proche allié
	/*
	std::vector<int> vecAllies = { index };
	closest = NULL;
	closest = (Animal*) addClosest(animal, vecAllies, inputs, true);
	// Ajout de la valeur att/def de l'animal en question aux inputs
	if (closest != NULL)
		inputs.push_back(closest->getCombatOutput());
	else
		inputs.push_back(0.f);
	//*/
	
	animal->update(inputs, dt);
}

Entity* EntityManager::addClosest(Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs, const bool isAnimal) {
	Position closest(worldSize * worldSize);
	Entity *entity = NULL;
	
	// find closest in all tabs
	for (unsigned int i = 0; i < speciesIndexes.size(); i++) {
		entity = getClosestFromTab(animal->getPos(), entities[ speciesIndexes[i] ].tab, closest, isAnimal);
	}
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	return entity;
}

Entity* EntityManager::addClosestFruit(Animal* animal, std::vector<float> &inputs) {
	Position closest(worldSize * worldSize);
	Entity *entity = NULL;
	
	// find closest fruit
	entity = getClosestFromTab(animal->getPos(), fruits, closest, false);
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	return entity;
}

Entity* EntityManager::getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest, const bool isAnimal) {
	Position tmp(worldSize * worldSize);
	Entity* entity = NULL;
	
	for (unsigned int i = 0; i < tab.size(); i++) {
		
		if (isAnimal && !((Animal*) tab[i])->isAlive())
			continue;
		
		tmp.pos = wrapPositionDifference(pos, tab[i]->getPos());
		tmp.dist = sqrt(tmp.pos.x * tmp.pos.x + tmp.pos.y * tmp.pos.y);
		
		if (tmp.dist < closest.dist) {
			closest = tmp;
			entity = tab[i];
		}
	}
	
	return entity;
}

const Vect2i EntityManager::wrapPositionDifference(const Vect2i a, const Vect2i b) {
	Vect2i tmp(b.x - a.x, b.y - a.y);
	
	if (abs(tmp.x) > worldSize / 2)
		tmp.x = -1 * sgn(tmp.x) * (worldSize - abs(tmp.x));
	if (abs(tmp.y) > worldSize / 2)
		tmp.y = -1 * sgn(tmp.y) * (worldSize - abs(tmp.y));
	
	return tmp;
}

const void EntityManager::addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle) {
	// Si p.dist = worldsize², cette position ne représente rien de réel (aucun closest n'a été trouvé par les autres fonctions)
	// Si p est invalide, on donne à l'animal des valeurs limites
	if (p.dist == worldSize * worldSize) {
		inputs.push_back(0.f);
		inputs.push_back(1.f);
	// Sinon
	} else {
		/*
		inputs.push_back(p.pos.x / p.dist);
		inputs.push_back(p.pos.y / p.dist);
		//*/
		// angle [0; 1[ ( [0; 360°[ ) relatif : angle du mobile - mon angle
		inputs.push_back(atan2f(p.pos.y, p.pos.x) / (2 * PI) - angle / 360.f);
		// distance as sigmoid so both [0; 1[
		inputs.push_back(1.f / (1.f + expf(-p.dist / distanceSigmoid)) * 2.f - 1.f);
		//*/
	}
}

void EntityManager::collisionCheck(Animal *animal, const int index) {
	// Si l'animal marche sur un fruit, il le mange et on break
	for (unsigned int i = 0; i < entities[fruitsIndex].tab.size(); i++) {
		Fruit *fruit = (Fruit*) entities[fruitsIndex].tab[i];
		
		if (isColliding(animal->getPos(), fruit->getPos())) {
			animal->incrementScore();
			fruit->init();
			
			break;
		}
	}
	
	int prayIndex;
	Animal *pray;
	// Si l'animal attaque lorsqu'il est dans la hitbox d'une proie, il la mange et on break
	// Parcours de toutes les proies
	for (unsigned int i = 0; i < entities[index].prays.size(); i++) {
		prayIndex = entities[index].prays[i];

		// Parcours de tous les animaux de l'espèce proie
		for (unsigned int j = 0; j < entities[prayIndex].tab.size(); j++) {
			pray = (Animal*) entities[prayIndex].tab[j];

			if (!pray->isAlive())
				continue;

			if (isColliding(animal->getPos(), pray->getPos()) && animal->getAttackRate() > pray->getDefenseRate()) {
				pray->die();
				animal->incrementScore();

				entities[prayIndex].aliveAnimals--;

				break;
			}
		}
	}
}

// Test de collision des bois : hitbox prédéfinie, carrée : simple, rapide, efficace (ou presque)
const bool EntityManager::isColliding(const Vect2i a, const Vect2i b) {
	return (abs(a.x - b.x) < hitbox) && (abs(a.y - b.y) < hitbox);
}
