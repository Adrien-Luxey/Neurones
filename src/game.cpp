#include "game.h"

Game::Game() 
: display(this), continuer(true), pause(false), generation(1), dt(0), elapsedTime(0), dtSum(0), frames(0), fps(0) {	
	for (int i = 0; i < CFG->readInt("AnimalsNumber"); i++) {
		for (int j = CHICKEN; j < TYPES_CNT; j++)
			entities.push_back(new Animal(j));
	}
}

Game::~Game() {
	for (unsigned int i = 0; i < entities.size(); i++)
		delete entities[i];
}

void Game::exec() {
	while (continuer) {
		dt = display.getElapsedTime();
		display.events();
		
		if (!pause) {
			update();
			display.update(entities);
		}
	}
}

void Game::update() {
	elapsedTime += dt;	
	dtSum += dt;
	frames += 1;
	
	if (dtSum > 1) {
		fps = frames/dtSum;
		frames = 0;
		dtSum = 0;
	}
	
	if (gameover()) {
		newGeneration();
		genetics.evolve(entities);
		elapsedTime = 0;
		std::cout << "Nouvelle génération calculée en " << display.getElapsedTime() << " secondes\n";
	}
	
	for (unsigned int i = 0; i < entities.size(); i++) {
		entities[i]->update(entities, dt);
	}
}

void Game::newGeneration() {
	std::cout << "Génération #" << generation++ << " : " << Stats::highScore(entities) << ", " << Stats::averageScore(entities) << ", " << Stats::totalScore(entities) << std::endl << Stats::printDetailledScore(entities) << std::endl;
}

bool Game::gameover() {
	if (elapsedTime > CFG->readInt("EpocDuration"))
		return true;
	
	bool allDead = true;
	for (unsigned int i = 0; i < entities.size(); i++)
		allDead &= ((Animal*) entities[i])->isDead();
	
	return allDead;
}

void Game::togglePause() {
	pause = !pause;
}
