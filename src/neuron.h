#ifndef NEURON_H
#define NEURON_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "config_parser.h"

// Quite outdated, used to serve if you don't set "UseNeuronSigmoid", but it would break most functionnality
#define SEUIL	0
#define	OUT_0	0
#define OUT_1	1


/**
 * @brief	The Neuron class, with represents a singleton, without backprop or anything
 * @param _inputsNumber	The inputs number of the neuron
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
     * @return A float representing the output
     */
	const float run(const std::vector<float> inputs);

	/**
	 * @brief	Updates the DNA of the neuron with the new one given by the GA
     * @param DNA	The new piece of ADN
     */
	void setDNA(const std::vector<float> &DNA);

	/**
     * @return the ADN of the neuron
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
	const bool USE_NEURON_SIGMOID;
};

#endif // NEURON_H
