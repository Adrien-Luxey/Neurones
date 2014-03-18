#include <stdlib.h>

#include "game.h"

int main(void) {	
	Game game;
	
	srand(time(NULL));
	
	game.loop();
	
	return EXIT_SUCCESS;
}
