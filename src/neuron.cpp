#include "neuron.h"

Neuron::Neuron(unsigned int _inputsNumber)
: inputsNumber(_inputsNumber), NEURON_SIGMOID(CFG->readFloat("NeuronSigmoid")) {
	initWeights();
}

Neuron::~Neuron() {}

void Neuron::initWeights() {
	std::uniform_real_distribution<float> random(-1, 1);
	
	weights.clear();
	
	// i <= inputs : the first weight is for the bias
	for (unsigned int i = 0; i <= inputsNumber; i++)
		weights.push_back(random(generator));
}

// Execution du calcul du neurone en fonction de inputs
const float Neuron::run(const std::vector<float> inputs) {
	// weights[0] = bias
	float sum = weights[0];
	
	// calcul de la combinaison linéraire weights * inputs
	for (unsigned int i = 0; i < inputsNumber && i < inputs.size(); i++)
		sum += weights[i+1]*inputs[i];
	
	// La sigmoide de sum renvoie un float ]-1;1[
	return 1/(1 + expf(-sum/NEURON_SIGMOID)) * 2.f - 1.f;
}

// Changement de l'ADN après passage de l'algo génétique
void Neuron::setDNA(const std::vector<float> &DNA) {
	for (unsigned int i = 0; i < DNA.size() && i < weights.size(); i++)
		weights[i] = DNA[i];
}

std::vector<float> Neuron::getDNA() {
    return weights;
}
