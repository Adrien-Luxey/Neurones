#include "entityManager.h"

// AnimalsNumber = entities[1].size() (idem pour les fruits)


// TODO : la couleuuur des animaux

EntityManager::EntityManager()
  : speciesNumber(CFG->readInt("SpeciesNumber")), praysNumber(speciesNumber/2), 
	distanceSigmoid(CFG->readFloat("DistanceSigmoid")), hitbox(CFG->readInt("Hitbox")) {
	
	Species tmp;

	// fruits
	fruitsIndex = entities.size();
	for (int j = 0; j < CFG->readInt("FruitsNumber"); j++)
		tmp.tab.push_back(new Fruit());
		
	entities.push_back(tmp);
	
	// animaux
	animalsIndex = entities.size();
	for (int i = 0; i < speciesNumber; i++) {
		tmp.tab.clear();
		tmp.prays.clear();
		tmp.predators.clear();
		
		// Ajout des animaux
		for (int j = 0; j < CFG->readInt("AnimalsNumber"); j++)
			tmp.tab.push_back(new Animal());
		
		// Ajout des index des prédateurs/proies
		// Une espèce mange praysNumber espèces après elle dans le tableau, et se fait manger par praysNumber avant elle
		for (int j = 0; j < praysNumber; j++) {
			tmp.prays.push_back(animalsIndex + (i + j + 1 - animalsIndex) % speciesNumber);
			tmp.predators.push_back(animalsIndex + (i - j - 1 - animalsIndex) % speciesNumber);
		}
		
		entities.push_back(tmp);
	}
}

EntityManager::~EntityManager() {
	// Destruction de toutes les entities
	for (int i = 0; i < entities.size(); i++) {
		for (int j = 0; j < entities[i].tab.size(); j++)
			delete entities[i].tab[j];
			
		entities[i].clear();
	}
	
	entities.clear();
}

void EntityManager::update(const float dt) {
	// Parcours de toutes les espèces
	for (int i = animalsIndex; i < animalsIndex + speciesNumber; i++) {
		// Parcours de tous les animaux de l'espece
		for (int j = 0; j < entities[i].tab.size(); j++) {
			Animal *animal = (Animal*) entities[i].tab[j];
			
			if (!animal->isAlive(dt))
				continue;
			
			update(animal, i, dt);
			
			collisionCheck(animal, i);
		}
	}
}

void EntityManager::update(Animal *animal, const int index, const float dt) {
	std::vector<float> inputs;
	Position closestFruit;
	
	// Angle de l'animal [0; 1[
	inputs.push_back(animal->getAngle() / 360.f);
	
	// Plus proche proie
	addClosest(animal, entities[index].prays, inputs, true);
	
	// Plus proche prédateur
	addClosest(animal, entities[index].predators, inputs, true);
	
	// Plus proche fruit
	getClosestFromTab(animal->getPos(), entities[ fruitsIndex ].tab, closestFruit, false);
	addNormalizedPosition(closestFruit, inputs);
	
	animal->update(inputs, dt);
}

void EntityManager::addClosest(Animal *animal, const std::vector<int> &speciesIndexes, std::vector<float> &inputs, const bool isAnimal) {
	Position closest(worldSize * worldSize);
	
	// find closest in all tabs
	for (int i = 0; i < speciesIndexes.size(); i++) {
		getClosestFromTab(animal->getPos(), entities[ speciesIndexes[i] ].tab, closest, isAnimal);
	}
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs);
}

void EntityManager::getClosestFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest, const bool isAnimal) {
	Position tmp;
	
	for (int i = 0; i < tab.size(); i++) {
		
		if (isAnimal && !((Animal*) tab[i])->isAlive())
			continue;
		
		tmp.pos = wrapPositionDifference(pos, tab[i]->getPos());
		tmp.dist = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
		
		if (tmp.dist < closest.dist)
			closest = tmp;
	}
}

const Vect2i EntityManager::wrapPositionDifference(const Vect2i a, const Vect2i b) {
	Vect2i tmp(b.x - a.x, b.y - a.y);
	
	if (abs(tmp.x) > worldSize / 2)
		tmp.x = -1 * sgn(tmp.x) * (worldSize - abs(tmp.x));
	if (abs(tmp.y) > worldSize / 2)
		tmp.y = -1 * sgn(tmp.y) * (worldSize - abs(tmp.y));
	
	return tmp;
}

const void EntityManager::addNormalizedPosition(const Position p, std::vector<float> &inputs) {
	
	// Si p.pos.x, cette position ne représente rien de réel (aucun closest n'a été trouvé par les autres fonctions)
	// Si p est invalide, on donne à l'animal es valeurs limites
	if (p.pos.x == worldSize * worldSize) {
		inputs.push_back(0.f);
		inputs.push_back(1.f);
	// Sinon
	} else {
		// angle as radians/2*PI and distance as sigmoid so both [0; 1[
		inputs.push_back(atan2f(p.pos.y, p.pos.x) / (2 * PI));
		inputs.push_back(1.f / (1.f + expf(-p.dist/distanceSigmoid)) * 2.f - 1.f);
	}
}

void EntityManager::collisionCheck(Animal *animal, const int index) {
	// Si l'animal marche sur un fruit, il le mange et on break
	for (int i = 0; i < entities[fruitsIndex].tab.size(); i++) {
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
	if (animal->isAttacking()) {
		// Parcours de toutes les proies
		for (int i = 0; i < entities[index].prays.size(); i++) {
			prayIndex = entities[index].prays[i];
			
			// Parcours de tous les animaux de l'espèce proie
			for (int j = 0; j < entities[prayIndex].tab.size(); j++) {
				pray = (Animal*) entities[prayIndex].tab[j];
				
				if (!pray->isAlive())
					continue;
				
				if (isColliding(animal->getPos(), pray->getPos())) {
					pray->die();
					animal->incrementScore();
					
					break;
				}
			}
		}
	}
}

// Test de collision des bois : hitbox prédéfinie, carrée : simple, rapide, efficace (ou presque)
const bool EntityManager::isColliding(const Vect2i a, const Vect2i b) {
	return abs(a.x - b.x) < hitbox && abs(a.y - b.y) < hitbox;
}