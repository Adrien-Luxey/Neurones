#include <stdlib.h>
#include <vector>
#include <iostream>

#include "game.h"

int main(void) {	
	Game game;
	
	srand(time(NULL));
	
	game.exec();
	
	return EXIT_SUCCESS;
}
