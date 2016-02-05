#include "neural_network.h"

NeuralNetwork::NeuralNetwork(unsigned int _inputsNumber, unsigned int _outputsNumber)
: inputsNumber(_inputsNumber), outputsNumber(_outputsNumber) {
	initLayers();
}

// Création des couches en fonction des inputs, outputs et des données de config
void NeuralNetwork::initLayers() {
	layers.clear();
	outputs.clear();
	
	// Couche input : inputsNumber neurones à inputsNumber entrées
	// (utiliser le même nombre de neurones que d'entrées pour cette couche est une convention reconnue)
	// On rajoute les sorties du tour précédent en entrée du nn - ou pas
	// A neural network has 2 layers of neurons, the 3rd is the input one
	//layers.push_back(Layer(inputsNumber, inputsNumber));
	
	// Création des couches cachées suivant les données de config
	unsigned int inputs = inputsNumber;
	for (int i = 0; i < CFG->readInt("HiddenLayersNumber"); i++) {
		layers.push_back(Layer(inputs, CFG->readInt("HiddenLayerSize")));
		inputs = CFG->readInt("HiddenLayerSize");
	}
	
	// Enfin, création de la dernière couche qui a autant de neurones que d'outputs (encore une fois c'est une convention)
	layers.push_back(Layer(inputs, outputsNumber));
}

// Run : execute le calcul de tout le NN en fonction d'inputs
std::vector<float> NeuralNetwork::run(const std::vector<float> inputs) {
	// on ajoute les sorties du tour précédent en entrée du nn (pour voir))
	//outputs.insert(outputs.end(), inputs.begin(), inputs.end());
	outputs = inputs;
	
	for (unsigned int i = 0; i < layers.size(); i++) {
		outputs = layers[i].run(outputs);
	}
	
	return outputs;
}

// Fonction servant à collecter l'ADN pour l'algo génétique
std::vector<float> NeuralNetwork::getDNA() {
	std::vector<float> DNA, layerDNA;
	
	for (unsigned int i = 0; i < layers.size(); i++) {
		layerDNA = layers[i].getDNA();
		DNA.insert(DNA.end(), layerDNA.begin(), layerDNA.end());
	}
	
	return DNA;
}

// Et mise à jour de l'ADN après passage de l'algo génétique
void NeuralNetwork::setDNA(const std::vector<float> &DNA) {
	std::vector<float>::const_iterator begin = DNA.begin(), end;
	
	for (unsigned int i = 0; i < layers.size(); i++) {
		end = begin + layers[i].getDNASize() - 1;
		
		layers[i].setDNA(std::vector<float>(begin, end));
		
		begin = end + 1;
	}
}
