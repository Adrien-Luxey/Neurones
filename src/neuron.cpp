#include "neuron.h"

Neuron::Neuron(unsigned int _inputsNumber)
: inputsNumber(_inputsNumber) {
	// + 1 pour le bias
	weights = new float[inputsNumber + 1];
	initWeights();
}

Neuron::~Neuron() {
	delete[] weights;
}

// Initialisation des poids random [-1; 1] en fonction de inputsNumber
void Neuron::initWeights() {
	std::uniform_real_distribution<float> random(-1, 1);
	
	// !!! i <= inputs !!! (le premier weight est le bias)
	for (unsigned int i = 0; i <= inputsNumber; i++)
		weights[i] = random(generator);
}

// Execution du calcul du neurone en fonction de inputs
const float Neuron::run(const std::vector<float> inputs) {
	// weights[0] = bias
	float sum = weights[0];
	
	// calcul de la combinaison linéraire weights * inputs
	for (unsigned int i = 0; i < inputsNumber && i < inputs.size(); i++)
		sum += weights[i+1]*inputs[i];
	
	if (CFG->readInt("UseNeuronSigmoid")) {
		// La sigmoide de sum renvoie un float ]-1;1[
		return 1/(1 + expf(-sum/CFG->readFloat("NeuronSigmoid"))) * 2.f - 1.f;
	} else {
		// Sinon on effectue une fonction scale
		return (sum > SEUIL) ? OUT_1 : OUT_0;
	}
}

// Changement de l'ADN après passage de l'algo génétique
void Neuron::setDNA(const std::vector<float> &DNA) {
	for (unsigned int i = 0; i < inputsNumber && i < DNA.size(); i++)
		weights[i] = DNA[i];
}

const std::vector<float> getDNA() {
    std::vector<float> v(weights, weights + (inputsNumber+1) * sizeof(float));
    
    return v;
}
