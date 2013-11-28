#ifndef NEURON_H
#define NEURON_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "configParser.h"

// Si UseNeuronSigmoid = 0, output du neuron donne : (sum > SEUIL) ? OUT_1, OUT_0;
#define SEUIL	0
#define	OUT_0	0
#define OUT_1	1

class Neuron {
	public :
		Neuron(unsigned int _inputsNumber);
		
		// Initialisation des poids random [-1; 1] en fonction de inputsNumber
		void initWeights();
		
		// Execution du calcul du neurone en fonction de inputs
		const float run(const std::vector<float> inputs);
		
		// Changement de l'ADN après passage de l'algo génétique
		void setDNA(const std::vector<float> &DNA);
		
		// Fonctions servant à récupérer tout l'ADN du NN
		const std::vector<float> getDNA() { return weights; }
		const unsigned int getDNASize() { return weights.size(); }
		
	private :
		unsigned int inputsNumber;
		std::vector<float> weights;
};

#endif // NEURON_H
