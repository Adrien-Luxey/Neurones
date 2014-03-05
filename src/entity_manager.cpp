#include "entity_manager.h"
#include "genetics.h"

// AnimalsNumber = species[1].size() (idem pour les fruits)


// TODO : la couleuuur des animaux
/* TODO : cette histoire de Species c'est stupide : considérer que tout le monde est un animal 
 *		  alors qu'on s'est emmerdé à faire une classe Entity and co.
 *		  Il faut un vector contenant prays et predators dont les indices correspondent à ceux des animaux.
 */


EntityManager::EntityManager()
  : DISTANCE_SIGMOID(CFG->readInt("DistanceSigmoid")), WORLD_SIZE(CFG->readInt("WorldSize")),
	BUSHES_NUMBER(CFG->readInt("BushesNumber")), BUSHES_MIN_SIZE(CFG->readInt("BushesMinSize")),
	BUSHES_MAX_SIZE(CFG->readInt("BushesMaxSize")), BATTLE_DEVIATION(CFG->readFloat("BattleDeviation")),
	ALLOW_FRIENDLY_FIRE(CFG->readInt("AllowFriendlyFire")), RANDOM_IN_BATTLES(CFG->readInt("RandomInBattles")) {
	
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
		
		species.push_back(tmp);
	}
	
	init();
	
	// fruits
	for (unsigned int j = 0; j < fruits.size(); j++)
		fruits[j]->init(bushes[ rand() % bushes.size() ]);
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
	// buissons !
	bushes.clear();
	for (int i = 0; i < BUSHES_NUMBER; i++) {
		Bush bush;
		
		float angle = (rand() % 360) * PI / 180.f;
		std::normal_distribution<float> normalRandDist(0, WORLD_SIZE/5);
		int dist = abs(normalRandDist(generator));

		bush.pos.x = std::min(WORLD_SIZE, std::max(0, (int) (WORLD_SIZE/2 + cosf(angle) * dist)));
		bush.pos.y = std::min(WORLD_SIZE, std::max(0, (int) (WORLD_SIZE/2 + sinf(angle) * dist)));
		bush.size = rand() % (BUSHES_MAX_SIZE - BUSHES_MIN_SIZE) + BUSHES_MIN_SIZE;
		
		bushes.push_back(bush);
	}
}

void EntityManager::update() {
	// Parcours de toutes les espèces
	for (unsigned int i = 0; i < species.size(); i++) {		
		// Parcours de tous les animaux de l'espece
		for (unsigned int j = 0; j < species[i].tab.size(); j++) {
			Animal *animal = species[i].tab[j];
			
			if (!animal->isAlive())
				continue;
			
			update(animal, i);
			
			handleCollisions(animal, i);
		}
	}
}

void EntityManager::update(Animal *animal, const unsigned int index) {
	std::vector<float> inputs;
	
	// Plus proche fruit
	addClosestFruit(animal, inputs);
	
	// Plus proche ennemi
	addClosestEnemy(animal, index, inputs);
	
	// Plus proche allié
	addClosestAlly(animal, index, inputs);
	
	animal->update(inputs);
}

void EntityManager::addClosestEnemy(const Animal *animal, const unsigned int index, std::vector<float> &inputs) {
	Position closest(WORLD_SIZE * WORLD_SIZE);
	Animal *enemy = NULL;
	
	// find closest in all tabs
	for (unsigned int i = 0; i < species.size(); i++) {
		if (i == index)
			continue;
		
		Animal *tmp = NULL;
		tmp = getClosestAnimalFromTab(animal, species[i].tab, closest, false);
		
		if (tmp != NULL)
			enemy = tmp;
	}
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	// variable de combat de l'ennemi
	if (enemy != NULL) {
		inputs.push_back(enemy->getBattleOutput());
	} else {
		inputs.push_back(0.f);
	}
}

void EntityManager::addClosestAlly(const Animal *animal, const unsigned int index, std::vector<float> &inputs) {
	Position closest(WORLD_SIZE * WORLD_SIZE);
	Animal *ally = getClosestAnimalFromTab(animal, species[index].tab, closest, true);

	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
	
	// variable de combat de l'ennemi
	if (ally != NULL) {
		inputs.push_back(ally->getBattleOutput());
	} else {
		inputs.push_back(0.f);
	}
}

void EntityManager::addClosestFruit(const Animal* animal, std::vector<float> &inputs) {
	Position closest(WORLD_SIZE * WORLD_SIZE), tmp;
	
	// find closest fruit	
	for (unsigned int i = 0; i < fruits.size(); i++) {		
		tmp.pos = wrapPositionDifference(animal->getPos(), fruits[i]->getPos());
		tmp.dist = sqrt(tmp.pos.x * tmp.pos.x + tmp.pos.y * tmp.pos.y) - animal->getRadius() - fruits[i]->getRadius();
		
		if (tmp.dist < closest.dist) {
			closest = tmp;
		}
	}
	
	// Give closest's angle/dist to network 
	addNormalizedPosition(closest, inputs, animal->getAngle());
}

Animal* EntityManager::getClosestAnimalFromTab(const Animal *animal, const std::vector<Animal*> &tab, Position &closestPos, bool animalInTab) {
	Position tmp(WORLD_SIZE * WORLD_SIZE);
	Animal* closestAnimal = NULL;
	
	for (unsigned int i = 0; i < tab.size(); i++) {		
		if (!tab[i]->isAlive())
			continue;
		
		// Ne pas récupérer un animal dont la distance vaut 0 si l'animal étudié appartient à la liste
		if (animalInTab && animal->getPos().x == tab[i]->getPos().x && animal->getPos().y == tab[i]->getPos().y)
			continue;
		
		tmp.pos = wrapPositionDifference(animal->getPos(), tab[i]->getPos());
		tmp.dist = sqrt(tmp.pos.x * tmp.pos.x + tmp.pos.y * tmp.pos.y) - animal->getRadius() - tab[i]->getRadius();
		
		if (tmp.dist < closestPos.dist) {
			closestPos = tmp;
			closestAnimal = tab[i];
		}
	}
	
	return closestAnimal;
}

Vect2i EntityManager::wrapPositionDifference(const Vect2i a, const Vect2i b) const {
	Vect2i tmp(b.x - a.x, b.y - a.y);
	
	if (abs(tmp.x) > WORLD_SIZE / 2)
		tmp.x = -1 * sgn(tmp.x) * (WORLD_SIZE - abs(tmp.x));
	if (abs(tmp.y) > WORLD_SIZE / 2)
		tmp.y = -1 * sgn(tmp.y) * (WORLD_SIZE - abs(tmp.y));
	
	return tmp;
}

void EntityManager::addNormalizedPosition(const Position &p, std::vector<float> &inputs, const float &angle) {
	// Si p.dist = worldsize², cette position ne représente rien de réel (aucun closest n'a été trouvé par les autres fonctions)
	// Si p est invalide, on donne à l'animal des valeurs limites
	if (p.dist == WORLD_SIZE * WORLD_SIZE) {
		inputs.push_back(0.f);
		inputs.push_back(1.f);
	// Sinon
	} else {
		// angle [0; 1[ ( [0; 360°[ ) relatif : angle du mobile - mon angle
		inputs.push_back(atan2f(p.pos.y, p.pos.x) / (2 * PI) - angle / 360.f);
		// distance as sigmoid so both [0; 1[
		inputs.push_back(1.f / (1.f + expf(-p.dist / DISTANCE_SIGMOID)) * 2.f - 1.f);
	}
}

void EntityManager::handleCollisions(Animal *animal, const unsigned int index) {
	// collisions with fruits
	for (unsigned int i = 0; i < fruits.size(); i++) {
		if (isColliding(animal, fruits[i])) {
			animal->incrementScore();
			fruits[i]->init(bushes[ rand() % bushes.size() ]);
		}
	}
	
	// collisions with enemies
	Animal *enemy;
	for (unsigned int i = 0; i < species.size(); i++) {
		// do not check your own species if no friendly fire
		if (!ALLOW_FRIENDLY_FIRE && index == i)
			continue;
		
		for (unsigned int j = 0; j < species[i].tab.size(); j++) {
			enemy = species[i].tab[j];
			
			if (!enemy->isAlive())
				continue;
			
			// do not check yourself if friendly fire is on
			if (index == i && enemy->getPos().x == animal->getPos().x && enemy->getPos().y == animal->getPos().y)
				continue;

			if (isColliding(animal, enemy)) {
				// exit if our animal died
				if (battle(animal, enemy))
					return;
			}
		}
	}
}

bool EntityManager::battle(Animal *animal, Animal *enemy) {
	if (RANDOM_IN_BATTLES) {
		std::normal_distribution<float> animalAttackRand(animal->getAttackValue(), BATTLE_DEVIATION);
		std::normal_distribution<float> enemyDefenseRand(enemy->getDefenseValue(), BATTLE_DEVIATION);
		std::normal_distribution<float> enemyAttackRand(enemy->getAttackValue(), BATTLE_DEVIATION);
		std::normal_distribution<float> animalDefenseRand(animal->getDefenseValue(), BATTLE_DEVIATION);

		if (animalAttackRand(generator) > enemyDefenseRand(generator)) {
			enemy->die();
			animal->incrementScore();
			
			return false;
		} else if (enemyAttackRand(generator) > animalDefenseRand(generator)) {
			animal->die();
			enemy->incrementScore();

			return true;
		}
	} else {
		if (animal->getAttackValue() > enemy->getDefenseValue()) {
			enemy->die();
			animal->incrementScore();

			return false;
		} else if (enemy->getAttackValue() > animal->getDefenseValue()) {
			animal->die();
			enemy->incrementScore();
			
			return true;
		}
	}
	
	return false;
}

bool EntityManager::isColliding(const Entity *a, const Entity *b) const {
	// return dist(a <-> b) < a.radius + b.radius
	return (((a->getPos().x - b->getPos().x) * (a->getPos().x - b->getPos().x) +
			 (a->getPos().y - b->getPos().y) * (a->getPos().y - b->getPos().y)) <
			(a->getRadius() + b->getRadius()) * (a->getRadius() + b->getRadius()));
}
