#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <cmath>
#include <iostream>

#include "utils.h"
#include "layer.h"

/**
 * @brief The NN class, containing LayersNumber (form config.cfg) layers of neurons, the layers size depending on inputs/ouptus/config
 * @param _inputsNumber		The inputs number of the NN
 * @param _outputsNumber	The outputs number
 * 
 * This class describes the brain of every animal.
 * It is a very simple Neural Network, containing at least an input layer of inputsNumber neurons, and an output layer of outputsNumber neurons. You can configure as much hidden layers you want (one is enough, it has been proven for non linear activation functions) of as many neurons you want (this factor makes quite interesting changes to the animals' behavior).
 * 
 * It's most interesting feature is that, it is a blackbox combinatory machine (same inputs give same outputs).
 * We can synthetise all the behavior of this machine by extracting its DNA : an organized float array that represents every weight of every neuron, even though the values don't mean anything to us. After this extraction is made, we can mix up the DNA using the Genetics class, and see evolution occur !
 * 
 * On the other hand, it sadly means that our animals only act instinctly, strictly following what their DNA tell them to do. This could be improved if we used some kind of heavier Neural Network, like a sequential one for start (sequential means that the machine remembers its previous state).
 */
class NeuralNetwork {
	public :
		NeuralNetwork(unsigned int _inputsNumber, unsigned int _outputsNumber);
		
		/**
		 * @brief	Initialisation of the different layers depending on inputs/outputs/config (for the hidden layer)
         */
		void initLayers();
		
		// Run : execute le calcul de tout le NN en fonction d'inputs
		/**
		 * @brief Runs the calculation of the outputs array depending on the inputs
         * @param inputs	The inputs array
         * @return	The outpus array
         */
		std::vector<float> run(const std::vector<float> inputs);
		
		/**
         * @return The complete DNA of the NN, as an array of float
         */
		std::vector<float> getDNA();
		
		/**
		 * @brief	Updates the DNA of all neurons with the new one, given by the genetics algorithm
         * @param DNA	The DNA to set
         */
		void setDNA(const std::vector<float> &DNA);
		
	private :
		unsigned int inputsNumber, outputsNumber;
		std::vector<Layer> layers;
		std::vector<float> outputs;
};

#endif // NEURAL_NETWORK_H
