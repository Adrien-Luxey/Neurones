#include "game.h"

Game::Game() 
  : display(this), continuer(true), pause(false), generation(1), dt(0), elapsedTime(0), dtSum(0), frames(0), fps(0), epocDuration(CFG->readInt("EpocDuration")) {	
	
}

Game::~Game() {}

void Game::exec() {
	while (continuer) {
		dt = display.getElapsedTime();
		display.events();
		
		if (!pause) {
			update();
			display.update(manager);
		}
	}
}

void Game::update() {	
	if (gameover())
		newGeneration();
	
	manager.update(dt);
}

void Game::newGeneration() {
	genetics.evolve(entities);
	elapsedTime = 0;
	
	// Reinitialise le timer de display
	display.getElapsedTime();
	
	std::cout << "Génération #" << generation++; << std::endl;
}

bool Game::gameover() {
	if (elapsedTime > epocDuration)
		return true;
	
	return false;
}

void Game::togglePause() {
	pause = !pause;
}

void Game::updateFps() {
	elapsedTime += dt;	
	dtSum += dt;
	frames++;
	
	if (dtSum > 1) {
		fps = frames/dtSum;
		frames = 0;
		dtSum = 0;
	}
}
