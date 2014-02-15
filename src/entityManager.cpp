#include "entityManager.h"
#include "genetics.h"

// AnimalsNumber = species[1].size() (idem pour les fruits)


// TODO : la couleuuur des animaux
/* TODO : cette histoire de Species c'est stupide : considérer que tout le monde est un animal 
 *		  alors qu'on s'est emmerdé à faire une classe Entity and co.
 *		  Il faut un vector contenant prays et predators dont les indices correspondent à ceux des animaux.
 */


EntityManager::EntityManager()
  : distanceSigmoid(CFG->readInt("DistanceSigmoid")), hitbox(CFG->readInt("Hitbox")), worldSize(CFG->readInt("WorldSize")),
	bushesNumber(CFG->readInt("BushesNumber")), bushesMinSize(CFG->readInt("BushesMinSize")),
	bushesMaxSize(CFG->readInt("BushesMaxSize")), combatDeviation(CFG->readFloat("CombatDeviation")) {
	
	Species tmp;
	
	// fruits
	fruits.clear();
	for (int j = 0; j < CFG->readInt("FruitsNumber"); j++) {
		fruits.push_back(new Fruit());
	}
	
	// animaux
	for (int i = 0; i < CFG->readInt("SpeciesNumber"); i++) {
		tmp.tab.clear();
		
		// Ajout des animaux
		for (int j = 0; j < CFG->readInt("AnimalsNumber"); j++)
			tmp.tab.push_back(new Animal());
		
		tmp.aliveAnimals = tmp.tab.size();
		
		species.push_back(tmp);
	}
	
	init();
	
	// fruits
	for (unsigned int j = 0; j < fruits.size(); j++)
		((Fruit*) fruits[j])->init(bushes[ rand() % bushes.size() ]);
}

EntityManager::~EntityManager() {
	// Destruction de toutes les species
	for (unsigned int i = 0; i < species.size(); i++) {
		for (unsigned int j = 0; j < species[i].tab.size(); j++)
			delete species[i].tab[j];
			
		species[i].tab.clear();
	}
	
	species.clear();
	
	for (unsigned int i; i < fruits.size(); i++)
		delete fruits[i];
	fruits.clear();
}

void EntityManager::init() {
	for (unsigned int i = 0; i < species.size(); i++)
		species[i].aliveAnimals = species[i].tab.size();
	
	// buissons !
	bushes.clear();
	for (int i = 0; i < bushesNumber; i++) {
		Bush bush;
		
		float angle = (rand() % 360) * PI / 180.f;
		std::normal_distribution<float> normalRandDist(0, worldSize/5);
		int dist = abs(normalRandDist(generator));

		bush.pos.x = std::min(worldSize, std::max(0, (int) (worldSize/2 + cosf(angle) * dist)));
		bush.pos.y = std::min(worldSize, std::max(0, (int) (worldSize/2 + sinf(angle) * dist)));
		bush.size = rand() % (bushesMaxSize - bushesMinSize) + bushesMinSize;
		
		bushes.push_back(bush);
	}
}

void EntityManager::update(const float dt) {
	// Parcours de toutes les espèces
	for (unsigned int i = 0; i < species.size(); i++) {		
		// Parcours de tous les animaux de l'espece
		for (unsigned int j = 0; j < species[i].tab.size(); j++) {
			Animal *animal = species[i].tab[j];
			
			if (!animal->isAlive(dt))
				continue;
			
			update(animal, i, dt);
			
			collisionCheck(animal, i);
		}
	}
}

bool EntityManager::gameover() const {
	unsigned int aliveSpecies = 0;
	
	for (unsigned int i = 0; i < species.size(); i++) {
		if (species[i].aliveAnimals > 0)
			aliveSpecies++;
	}
	
	return (species.size() == 1 && aliveSpecies == 0) ||
		(species.size() > 1 && aliveSpecies < 2);
}

void EntityManager::update(Animal *animal, const unsigned int index, const float dt) {
	std::vector<float> inputs;
	
	// Plus proche fruit
	addClosestFruit(animal, inputs);
	
	// Plus proche ennemi
	addClosestEnemy(animal, index, inputs);
	
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

void EntityManager::addClosestEnemy(Animal *animal, const unsigned int index, std::vector<float> &inputs) {
	Position closest(worldSize * worldSize);
	Animal *enemy = NULL;
	
	// find closest in all tabs
	for (unsigned int i = 0; i < species.size(); i++) {
		if (i == index)
			continue;
		
		Animal *tmp = NULL;
		tmp = getClosestAnimalFromTab(animal->getPos(), species[i].tab, closest);
		
		if (tmp != NULL)
			enemy = tmp;
	}
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	// variable de combat de l'ennemi
	if (enemy != NULL)
		inputs.push_back(enemy->getCombatOutput());
	else
		inputs.push_back(0.f);
}

void EntityManager::addClosestAlly(Animal *animal, const unsigned int index, std::vector<float> &inputs) {
	Position closest(worldSize * worldSize);
	Animal *ally = getClosestAnimalFromTab(animal->getPos(), species[index].tab, closest);

	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	// variable de combat de l'ennemi
	if (ally != NULL)
		inputs.push_back(ally->getCombatOutput());
	else
		inputs.push_back(0.f);
}

void EntityManager::addClosestFruit(Animal* animal, std::vector<float> &inputs) {
	Position closest(worldSize * worldSize);
	
	// find closest fruit
	getClosestEntityFromTab(animal->getPos(), fruits, closest);
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
}

Entity* EntityManager::getClosestEntityFromTab(const Vect2i pos, const std::vector<Entity*> &tab, Position &closest) {
	Position tmp(worldSize * worldSize);
	Entity* entity = NULL;
	
	for (unsigned int i = 0; i < tab.size(); i++) {		
		tmp.pos = wrapPositionDifference(pos, tab[i]->getPos());
		tmp.dist = sqrt(tmp.pos.x * tmp.pos.x + tmp.pos.y * tmp.pos.y);
		
		if (tmp.dist < closest.dist) {
			closest = tmp;
			entity = tab[i];
		}
	}
	
	return entity;
}

Animal* EntityManager::getClosestAnimalFromTab(const Vect2i pos, const std::vector<Animal*> &tab, Position &closest) {
	Position tmp(worldSize * worldSize);
	Animal* entity = NULL;
	
	for (unsigned int i = 0; i < tab.size(); i++) {		
		if (!tab[i]->isAlive())
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
		// angle [0; 1[ ( [0; 360°[ ) relatif : angle du mobile - mon angle
		inputs.push_back(atan2f(p.pos.y, p.pos.x) / (2 * PI) - angle / 360.f);
		// distance as sigmoid so both [0; 1[
		inputs.push_back(1.f / (1.f + expf(-p.dist / distanceSigmoid)) * 2.f - 1.f);
	}
}

void EntityManager::collisionCheck(Animal *animal, const unsigned int index) {
	// Si l'animal marche sur un fruit, il le mange et on break
	for (unsigned int i = 0; i < fruits.size(); i++) {
		if (isColliding(animal->getPos(), fruits[i]->getPos())) {
			animal->incrementScore();
			((Fruit*) fruits[i])->init(bushes[ rand() % bushes.size() ]);
		}
	}
	
	Animal *enemy;
	// Si l'animal attaque lorsqu'il est dans la hitbox d'une proie, il la mange et on break
	// Parcours de toutes les proies
	for (unsigned int i = 0; i < species.size(); i++) {
		if (i == index)
			continue;
		
		// Parcours de tous les animaux de l'espèce proie
		for (unsigned int j = 0; j < species[i].tab.size(); j++) {
			enemy = species[i].tab[j];

			if (!enemy->isAlive())
				continue;
			
			std::normal_distribution<float> attackRand(animal->getAttackRate(), combatDeviation);
			std::normal_distribution<float> defenseRand(enemy->getDefenseRate(), combatDeviation);

			if (isColliding(animal->getPos(), enemy->getPos()) && attackRand(generator) > defenseRand(generator)) {
				enemy->die();
				animal->incrementScore();

				species[i].aliveAnimals--;

				break;
			}
		}
	}
}

// Test de collision des bois : hitbox prédéfinie, carrée : simple, rapide, efficace (ou presque)
const bool EntityManager::isColliding(const Vect2i a, const Vect2i b) {
	return (abs(a.x - b.x) < hitbox) && (abs(a.y - b.y) < hitbox);
}
