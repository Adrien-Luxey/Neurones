#ifndef GENETICS_H
#define GENETICS_H

#include <vector>
#include <iostream>

#include "utils.h"
#include "entity_manager.h"
#include "entity.h"

class Game;

/**
 * @brief More or less static class operating the genetic algorithm on all the animal species of the EntityManager
 */
class Genetics {
  public:

	Genetics();
	/**
	 * @brief Entry point of the GA
     * @param manager	the EnttyManager where to find the species to evolve
     */
	void evolve(EntityManager &manager);

  private:
	// Structure contenant l'ADN, le score et le score cumulé (pour la roulette wheel) de chaque animal
	/// @brief Structure used by the GA to store the DNA ans score of every animal
	typedef struct AnimalData {
		std::vector<float> DNA;			///< The animal DNA
		unsigned int score;				///< The animal score
		unsigned int cumulatedScore;	///< The cumulated socre of the animal (to be used by the roulette wheel))
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
