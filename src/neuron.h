#ifndef NEURON_H
#define NEURON_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "config_parser.h"

/**
 * @brief	The Neuron class, with represents a singleton, without backprop or anything
 * @param _inputsNumber	The inputs number of the neuron
 * 
 * The Neurons used in the projects are ultra simple percpetrons, without back-propagation or anything like it.
 * 
 * It uses sigmoids as an activation function, because floats are so cooler than bools (and much more CPU-intensive).
 * 
 * I'll let you check out Wikipedia if you need more info about how a perceptron work, or let you check out the NeuralNetwork class, where I described a bit more how my Neural Networks worked.
 */
class Neuron {
  public:	
	Neuron(unsigned int _inputsNumber);
	~Neuron();

	/**
	 * @brief Initializes all the weights with uniform rand values [-1; 1]
     */
	void initWeights();

	/**
	 * @brief	Runs the calculation of the output value depending on the inputs
     * @param inputs	The inputs array
     * @return The output value 
	 * 
	 * To compute the output, we make the linear combination of weights and inputs : sum = weights * inputs
	 * Then, we apply an activation function to sum, so that it stays within the interval [-1; 1]
	 * In our case, we use a sigmoid, which inclination is determined by NeuronSigmoid in config.cfg
     */
	const float run(const std::vector<float> inputs);

	/**
	 * @brief	Updates the DNA of the neuron with the new one given by the GA
     * @param DNA	The new piece of ADN
	 * 
	 * We simply replace the actual weights by the ones given by the DNA
     */
	void setDNA(const std::vector<float> &DNA);

	/**
     * @return the DNA of the neuron, which is the array of its weights
     */
	std::vector<float> getDNA();

	/**
     * @return The size of the DNA of the neuron
     */
	unsigned int getDNASize() const { return weights.size(); }

  private:
	unsigned int inputsNumber;
	std::vector<float> weights;

	const float NEURON_SIGMOID;
};

#endif // NEURON_H
