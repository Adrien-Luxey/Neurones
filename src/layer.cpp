#include "layer.h"

Layer::Layer(unsigned int _inputsNumber, unsigned int _neuronsNumber)
: inputsNumber(_inputsNumber), neuronsNumber(_neuronsNumber) {
	
	initNeurons();
}

Layer::~Layer() {}

// Initialisation des neurones de la couche
void Layer::initNeurons() {	
	neurons.clear();
	
	for (unsigned int i = 0; i < neuronsNumber; i++)
		neurons.push_back(Neuron(inputsNumber));
	
	outputs.clear();
	outputs.resize(neuronsNumber);
}

// Execution de tous les neurones en fonction de inputs
std::vector<float> Layer::run(const std::vector<float> inputs) {
	for (unsigned int i = 0; i < neuronsNumber && i < inputs.size(); i++) {
		outputs[i] = neurons[i].run(inputs);
	}
	
	return outputs;
}

// Fonctions servant à reconstituer l'ADN complet du NN
std::vector<float> Layer::getDNA() {
	std::vector<float> DNA;
	
	for (unsigned int i = 0; i < neuronsNumber; i++) {
		std::vector<float> neuronDNA = neurons[i].getDNA();
		DNA.insert(DNA.end(), neuronDNA.begin(), neuronDNA.end());
	}
	
	return DNA;
}
unsigned int Layer::getDNASize() const {
	unsigned int DNASize = 0;

	for (unsigned int i = 0; i < neuronsNumber; i++)
		DNASize += neurons[i].getDNASize();
	
	return DNASize;
}

// MaJ de l'ADN du NN après passage de l'algo génétique
void Layer::setDNA(const std::vector<float> &DNA) {
	std::vector<float>::const_iterator begin = DNA.begin(), end;
	
	for (unsigned int i = 0; i < neuronsNumber; i++) {
		end = begin + neurons[i].getDNASize() - 1;
		
		neurons[i].setDNA(std::vector<float>(begin, end));
		
		begin = end + 1;
	}
}
