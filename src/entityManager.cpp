#include "entityManager.h"

// AnimalTypes = entities.size() - 1 (les plantes)
// AnimalsNumber = entities[1].size() (idem pour les fruits)


// TODO : la couleuuur des animaux

EntityManager::EntityManager() {
	std::vector<Entity*> tmp;
	speciesNumber = CFG->readInt("SpeciesNumber");

	// fruits
	fruitsIndex = entities.size();
	for (int j = 0; j < CFG->readInt("FruitsNumber"); j++)
		tmp.push_back(new Fruit());
		
	entities.push_back(tmp);
	
	// animaux
	animalsIndex = entities.size();
	for (int i = 0; i < speciesNumber; i++) {
		tmp.clear();

		for (int j = 0; j < CFG->readInt("AnimalsNumber"); j++)
			tmp.push_back(new Animal());
		    
		entities.push_back(tmp);
	}
}

EntityManager::~EntityManager() {
	for (int i = 0; i < entities.size(); i++) {
		for (int j = 0; j < entities[i].size(); j++)
			delete entities[i][j];
			
		entities[i].clear();
	}
	
	entities.clear();
}

void EntityManager::update(const float dt) {
	for (int i = animalsIndex; i < speciesNumber; i++) {
		for (int j = 0; j < entities[i].size(); j++) {
			Animal *animal = (Animal*) entities[i][j];
			
			if (!animal->isAlive(dt))
				continue;
			
			
		}
	}
}

void EntityManager::update(const Animal *animal, const int index, const float dt) {
	
}