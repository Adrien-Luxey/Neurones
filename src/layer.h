#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "config_parser.h"
#include "neuron.h"

/**
 * @brief Class representing a layer of neurons inside the neural network
 * @param _inputsNumber		Number of inputs for each neuron
 * @param _neuronsNumber	Number of outputs for each neuron
 * 
 * The wikipedia page of Neural Networks will explain you better what a layer is, so go check out if you have any doubt.
 * Neuron and NeuralNetwork are more interesting classes to investigates, Layer is only an intermediate.
 */
class Layer {
	public :
		Layer(unsigned int _inputsNumber, unsigned int _neuronsNumber);
		~Layer();
		
		/**
		 * @brief initialize the neurones of the layer with the inputs/outputs numbers
         */
		void initNeurons();
		
		/**
		 * @brief Execution of all the neurons of the layer according to the inputs array
         * @param inputs	The inputs array
         * @return A float array representing the outputs
         */
		std::vector<float> run(const std::vector<float> inputs);
		
		/**
		 * 
         * @return the DNA of all the neurons as a float vector
         */
		std::vector<float> getDNA();
		
		/**
		 * 
         * @return The DNA of all the neurons size
         */
		unsigned int getDNASize() const;
		
		/**
		 * @brief Update the neurons' DNA with the news value
         * @param DNA	The new DNA to set
         */
		void setDNA(const std::vector<float> &DNA);
		
	private :
		unsigned int inputsNumber, neuronsNumber;
		std::vector<Neuron> neurons;
		std::vector<float> outputs;
};

#endif // LAYER_H
