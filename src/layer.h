#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "configParser.h"
#include "neuron.h"

class Layer {
	public :
		Layer(unsigned int _inputsNumber, unsigned int _neuronsNumber);
		~Layer();
		
		// Initialisation des neurones de la couche
		void initNeurons();
		
		// Execution de tous les neurones en fonction de inputs
		std::vector<float> run(const std::vector<float> inputs);
		
		// Fonctions servant à reconstituer l'ADN complet du NN
		const std::vector<float> getDNA();
		
		const unsigned int getDNASize();
		
		// MaJ de l'ADN du NN après passage de l'algo génétique
		void setDNA(const std::vector<float> &DNA);
		
	private :
		unsigned int inputsNumber, neuronsNumber;
		Neuron neurons[];
		std::vector<float> outputs;
};

#endif // LAYER_H
