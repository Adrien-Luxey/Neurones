#ifndef GENETICS_H
#define GENETICS_H

#include <vector>
#include <iostream>

#include "utils.h"
#include "entity_manager.h"
#include "entity.h"

class Game;

class Genetics {
  public:

	Genetics();
	void evolve(EntityManager &manager);

  private:
	// Structure contenant l'ADN, le score et le score cumulé (pour la roulette wheel) de chaque animal
	typedef struct AnimalData {
		std::vector<float> DNA;
		unsigned int score;
		unsigned int cumulatedScore;
	} AnimalData;
	
	void roulette();
	void truncation(std::vector<Entity*> entities);
	void elitism();
	void crossover();
	void mutation(std::vector<float> &DNA);

	void sortData(std::vector<AnimalData> &data, const int asc);

	std::vector<AnimalData> children, parents, animalsData;
	
	const int CROSSOVER_RATE;
	const float MUTATION_GAUSS_DEVIATION;
};

#endif // GENETICS_H
