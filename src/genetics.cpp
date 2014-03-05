#include "genetics.h"

Genetics::Genetics()
  : CROSSOVER_PROBABILITY(CFG->readInt("CrossoverProbability")), MUTATION_PROBABILITY(CFG->readInt("MutationProbability")),
	MUTATION_GAUSS_DEVIATION(CFG->readFloat("MutationGaussDeviation")) {}

void Genetics::evolve(EntityManager &manager) {
	std::vector<Species> species = manager.getSpecies();
	
	// for each species
	for (unsigned int i = 0; i < species.size(); i++) {	
		std::vector<AnimalData> children, parents;
		
		selection(species[i].tab, parents);
		
		while (children.size() < parents.size()) {
			unsigned int fatherIndex = roulette(parents), motherIndex = roulette(parents);
			
			crossover(parents, fatherIndex, motherIndex, children);
		}
	
		// initialise the animals with all these new DNAs
		for (unsigned int j = 0; j < children.size(); j++) {
			species[i].tab[j]->init(children[j].DNA);
		}
	}
}

void Genetics::selection(const std::vector<Animal*> &animals, std::vector<AnimalData> &parents) {
	unsigned int cumulated = 0;
	
	for (unsigned int i = 0; i < animals.size(); i++) {
		AnimalData tmp;

		tmp.score = animals[i]->getScore();
		tmp.DNA = animals[i]->getDNA();

		cumulated += tmp.score + 1;
		tmp.cumulatedScore = cumulated;

		parents.push_back(tmp);
	}
}


unsigned int Genetics::roulette(const std::vector<AnimalData> &array) const {
	std::uniform_int_distribution<int> random(0, array[array.size() - 1].cumulatedScore);
	unsigned int randomValue = random(generator), index = 0;
	
	while (index < array.size() && randomValue > array[index].cumulatedScore)
		index++;
	
	return index;
}

void Genetics::crossover(const std::vector<AnimalData> &parents, const unsigned int fatherIndex,
						 const unsigned int motherIndex, std::vector<AnimalData> &children) {
	
	std::uniform_int_distribution<int> randomParent(0, 1);
	std::uniform_int_distribution<int> crossoverProbability(0, 100);
	
	// We generate 2 children if possible
	if (parents.size() - children.size() >= 2) {
		AnimalData child1, child2;
		
		// CROSSOVER_PROBABILITY % chances that we apply crossover
		if (crossoverProbability(generator) <= CROSSOVER_PROBABILITY) {
			// In selection, we do not verify that the 2 parents are different
			// Let's avoid doing a expensive crossover if we are only copying one DNA
			if (fatherIndex == motherIndex) {
				child1.DNA = parents[fatherIndex].DNA;
				child2.DNA = parents[fatherIndex].DNA;
			} else {
				// uniform crossover of the 2 parents' DNA
				for (unsigned int j = 0; j < parents[0].DNA.size(); j++) {
					if (randomParent(generator) == 0) {
						child1.DNA.push_back(parents[fatherIndex].DNA[j]);
						child2.DNA.push_back(parents[motherIndex].DNA[j]);
					} else {
						child1.DNA.push_back(parents[motherIndex].DNA[j]);
						child2.DNA.push_back(parents[fatherIndex].DNA[j]);
					}
				}
			}
		} else {
			// no crossover
			child1.DNA = parents[fatherIndex].DNA;
			child2.DNA = parents[motherIndex].DNA;
		}

		// Mutation of the newborns
		mutation(child1.DNA);
		mutation(child2.DNA);

		children.push_back(child1);
		children.push_back(child2);

	// Otherwise we only create one child
	} else {
		AnimalData child;
		
		// CROSSOVER_PROBABILITY % chances that we apply crossover
		if (crossoverProbability(generator) <= CROSSOVER_PROBABILITY) {
			// In selection, we do not verify that the 2 parents are different
			// Let's avoid doing a expensive crossover if we are only copying one DNA
			if (fatherIndex == motherIndex) 
				child.DNA = parents[fatherIndex].DNA;
			else {
				// uniform crossover
				for (unsigned int j = 0; j < parents[0].DNA.size(); j++) {
					if (randomParent(generator) == 0) {
						child.DNA.push_back(parents[fatherIndex].DNA[j]);
					} else {
						child.DNA.push_back(parents[motherIndex].DNA[j]);
					}
				}
			}
		} else {
			// no crossover, we choose one random parent and copy his DNA
			if (randomParent(generator) == 0)
				child.DNA = parents[fatherIndex].DNA;
			else
				child.DNA = parents[motherIndex].DNA;
		}

		mutation(child.DNA);

		children.push_back(child);
	}
}

void Genetics::mutation(std::vector<float> &DNA) {
	std::uniform_int_distribution<int> mutationProbability(0, 100);
	
	// MUTATION_PROBABILITY % chances that we apply mutation
	if (mutationProbability(generator) <= MUTATION_PROBABILITY) {
		// we mutate exactly one gene from the DNA
		std::uniform_int_distribution<int> randGene(0, DNA.size() - 1);
		int geneIndex = randGene(generator);
		
		std::normal_distribution<float> gauss(DNA[geneIndex], MUTATION_GAUSS_DEVIATION);
		DNA[geneIndex] = gauss(generator);
	}
}
