/**
 * \file main.cpp
 * \author Adrien Luxey
 * \brief This is the entry point of the Neurones program.
 * 
 * Neurones is a Neural Network / Genetic Algorithm simulation, intended to play around
 * with this awesome subject that is AI learning, in the hope to find new and better
 * ways to use it, and explore its capabilities.
 */

#include <stdlib.h>

#include "game.h"

int main(void) {	
	Game game;
	
	srand(time(NULL));
	
	game.loop();
	
	return EXIT_SUCCESS;
}
