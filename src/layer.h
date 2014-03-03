#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "config_parser.h"
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
		std::vector<float> getDNA();
		
		unsigned int getDNASize() const;
		
		// MaJ de l'ADN du NN après passage de l'algo génétique
		void setDNA(const std::vector<float> &DNA);
		
	private :
		unsigned int inputsNumber, neuronsNumber;
		std::vector<Neuron> neurons;
		std::vector<float> outputs;
};

#endif // LAYER_H
