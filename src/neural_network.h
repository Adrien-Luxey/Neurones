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
