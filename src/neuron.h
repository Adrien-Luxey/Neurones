#ifndef NEURON_H
#define NEURON_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "config_parser.h"

// Si UseNeuronSigmoid = 0, output du neuron donne : (sum > SEUIL) ? OUT_1, OUT_0;
#define SEUIL	0
#define	OUT_0	0
#define OUT_1	1

class Neuron {
  public:	
	Neuron(unsigned int _inputsNumber);
	~Neuron();

	// Initialisation des poids random [-1; 1] en fonction de inputsNumber
	void initWeights();

	// Execution du calcul du neurone en fonction de inputs
	const float run(const std::vector<float> inputs);

	// Changement de l'ADN après passage de l'algo génétique
	void setDNA(const std::vector<float> &DNA);

	// Fonctions servant à récupérer tout l'ADN du NN
	std::vector<float> getDNA();

	unsigned int getDNASize() const { return weights.size(); }

  private:
	unsigned int inputsNumber;
	std::vector<float> weights;

	const float NEURON_SIGMOID;
	const bool USE_NEURON_SIGMOID;
};

#endif // NEURON_H
