#ifndef GENETICS_H
#define GENETICS_H

#include <vector>
#include <iostream>

#include "utils.h"
#include "entity.h"

class Game;

// Structure contenant l'ADN, le score et le score cumulé (pour la roulette wheel) de chaque animal
typedef struct AnimalData {
	std::vector<float> DNA;
	unsigned int score;
	unsigned int cumulatedScore;
	unsigned int type;
} AnimalData;

class Genetics {
	public :	
		Genetics() {}
		void evolve(std::vector<Entity*> entities);
		
	private :
		void roulette(const int type);
		void truncation(std::vector<Entity*> entities);
		void elitism();
		void crossover();
		void mutation(std::vector<float> &DNA);
		
		void sortData(std::vector<AnimalData> &data, const int asc);
		
		std::vector<AnimalData> children, parents, animalsData;
};

#endif // GENETICS_H
