#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <cmath>
#include <iostream>

#include "utils.h"
#include "layer.h"

// Ajout de la backpropagation : on rajoute les précédentes sorties en entrées

// Réseau de neurones, comprenant les diverses couches de neurones, elles meme constituées de neurones
class NeuralNetwork {
	public :
		NeuralNetwork(unsigned int _inputsNumber, unsigned int _outputsNumber);
		
		// Création des couches en fonction des inputs, outputs et des données de config
		void initLayers();
		
		// Run : execute le calcul de tout le NN en fonction d'inputs
		const std::vector<float> run(const std::vector<float> inputs);
		
		// Fonction servant à collecter l'ADN pour l'algo génétique
		const std::vector<float> getDNA() const;
		
		// Et mise à jour de l'ADN après passage de l'algo génétique
		void setDNA(const std::vector<float> &DNA);
		
	private :
		unsigned int inputsNumber, outputsNumber;
		std::vector<Layer> layers;
		std::vector<float> outputs;
};

#endif // NEURAL_NETWORK_H
