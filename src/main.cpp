#include <stdlib.h>
#include <vector>

#include "game.h"

int main(void) {
	Game game;
	
	/*Layer layer(4, 4);
	std::vector<float> inputs;
	inputs.push_back(0);
	inputs.push_back(1);
	inputs.push_back(0);
	inputs.push_back(1);
	
	layer.description();
	layer.run(inputs);
	layer.description();
	
	std::vector<float> dna = layer.getDNA();
	
	for (int i = 0; i < dna.size(); i++)
		std::cout << dna[i] << " ";
	std::cout << std::endl;*/
	
	srand(time(NULL));
	
	game.exec();
	
	return EXIT_SUCCESS;
}
