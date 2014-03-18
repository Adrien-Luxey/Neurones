#ifndef GENETICS_H
#define GENETICS_H

/**
 * @file genetics.h
 * @author	Adrien Luxey
 */

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "utils.h"
#include "entity_manager.h"
#include "entity.h"

class Game;

/**
 * @brief More or less static class operating the genetic algorithm on all the animal species of the EntityManager
 * 
 * I am not here to discuss the pros and cons of the different implementations of selection, rossover and mutation, however I have quite a lot to say about it.
 * Fear not, I might (someday) right some things about these (very interesting) subjects. But for that I need DATA !
 * My software doesn't provide any at the time, and every conclusion I might have made is only a result of a subjective observation. 
 * 
 * Likewise, I won't here explain the internal working of the algorithms I used. You have the code right here, and the web is full of documentation.
 * Googling "rank-based selection", "uniform crossover" and such will teach you everything you need to know.
 * Again, I might right something about all this someday.
 * However, if you have interesting data on the different mutation operators that work fine on real values, I'm interested !
 * 
 * Sidenotes of the day :
 * - Mutation deviation should be variable, but how ?
 * 
 */
class Genetics {
  public:

	Genetics();
	/**
	 * @brief Entry point of the GA
     * @param manager	the EntityManager where to find the species to evolve
	 * 
	 * Performs the genetic algorithm on every species of the manager
     */
	void evolve(EntityManager &manager);

  private:
	/// @brief Structure used by the GA to store the DNA ans score of every animal
	typedef struct AnimalData {
		std::vector<float> DNA;			///< The animal DNA
		unsigned int score;				///< The animal score
		unsigned int selectionScore;	///< The cumulated socre of the animal (to be used by the roulette wheel)
	} AnimalData;
	
	
	/**
	 * @brief Creation of the parents array using a rank based selection.
     * @param animals	The input animals array
     * @param parents	The output parents array
	 * 
	 * We here create the parents array using the animals array.
	 * We perform a rank based selection, filling selectionScore with the right input for the roulette function
     */
	void selection(const std::vector<Animal*> &animals, std::vector<AnimalData> &parents);
	
	/**
	 * @brief roulette wheel algorithm
     * @param array	The AnimalData input array
     * @return An individual randomly choosen by the roulette
	 * 
	 * The algorithm uses the selectionScore value calculated by selection to randomly choose one individual from the input array.
	 * Please note we don't check the first parent's index value. ie both parents could be the same, resulting in no crossover.
     */
	unsigned int roulette(const std::vector<AnimalData> &array) const;
	
	/**
	 * @brief Performs a uniform crossover of both parents to create new individuals
     * @param parents		the parents array
     * @param fatherIndex	The index of the first parent in the parents array
     * @param motherIndex	The index of the second parent in the parents array
	 *						(I am pro gay marriage, I just needed a var name. Leave me alone.)
     * @param children		The output children array to fill.
	 *						(About my gay marriage note. I'm sorry. French integrists make me paranoid. Come by someday, we'll have coffee.)
	 * 
	 * We here use the CrossoverProbability constant defined in config.cfg.
	 * After every newborn is made, we call mutation.
     */
	void crossover(	const std::vector<AnimalData> &parents, const unsigned int fatherIndex,
					const unsigned int motherIndex, std::vector<AnimalData> &children);
	
	/**
	 * @brief Performs a mutation on a random gene of the given DNA
     * @param DNA	The input DNA
	 * 
	 * As for crossover, we use MutationProbability to choose wether or not mutate the DNA.
	 * We chose to mutate exactly one random gene from the DNA. It's the least expensive operation, and it's not worse than other solutions.
	 * 
	 * Using real numbers in genes, the best solution when mutating is to use a gaussian function.
	 * This way, we know that the new gene's value won't be far away from the old one.
	 * To control the amplitude of the mutation, we use the constant MutationGaussDeviation, in config.cfg.
	 * This constant is crucial. Set high, it enhances exploration of new solutions. Set low, it narrows the solutions to a local maximum.
	 * Because a high mutation breaks potentially good solutions, it would be nice to see the deviation reduce after some generations.
     */
	void mutation(std::vector<float> &DNA);
	
	/**
	 * @brief Static ascending score compare function to be used by stdlib's qsort
     * @param a	First Animal
     * @param b	Second Animal
     * @return a.score - b.score
	 * 
     */
	static int compareAnimalData(const void *a, const void *b);

	std::vector<AnimalData> children, parents, animalsData;
	
	const int CROSSOVER_PROBABILITY, MUTATION_PROBABILITY;
	const float MUTATION_GAUSS_DEVIATION;
};

#endif // GENETICS_H
