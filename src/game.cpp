#include "game.h"


Game::Game() 
  : display(this), continuer(true), pause(false), displayed(true), generation(1), iterationsPerGeneration(CFG->readInt("IterationsPerGeneration")),
	gameSpeed(1), dtSum(0), realGameSpeed(0), iterations(0), loopIterations(0) {	
	
}

Game::~Game() {}

void Game::exec() {
	while (continuer) {
		display.events();
		
		if (!pause) {
			if (!displayed || (gameSpeed >= 1.f) || (loopIterations % (int) (1.f / gameSpeed) == 0))
				update();
			
			if (displayed && ((gameSpeed <= 1.f) || (loopIterations % (int) gameSpeed == 0)))
				display.update(manager);
			
			updateGameSpeed();
			
			loopIterations++;
		}
	}
}

void Game::togglePause() {
	pause = !pause;
}

void Game::increaseGameSpeed() {
	gameSpeed = (gameSpeed >= 1) ? gameSpeed + 1 : gameSpeed*2;
}
void Game::decreaseGameSpeed() {
	gameSpeed = (gameSpeed > 1) ? gameSpeed - 1 : gameSpeed/2;
}

void Game::newGeneration() {
	genetics.evolve(manager);
	manager.init();
	
	iterations = 0;
	loopIterations = 0;
	
	display.getElapsedTime();
	
	std::cout << "Génération #" << generation++ << std::endl;
}

void Game::update() {
	if (gameover())
		newGeneration();
	
	// dt * gameSpeed => le manager pense que le frameRate est toujours le même qu'à gamespeed = 1
	// Sinon la vitesse des mobiles ne serait pas accélérée
	manager.update();
	
	iterations++;
	//realGameSpeed =  1.f / (DEFAULT_FPS * display.getElapsedTime());
	iterationsSum++;
}

bool Game::gameover() {	
	if (iterations > iterationsPerGeneration || manager.gameover())
		return true;
	
	return false;
}

void Game::updateGameSpeed() {
	dtSum += display.getElapsedTime();
	
	if (dtSum >= 1.f) {
		realGameSpeed = iterationsSum / DEFAULT_FPS;
		
		if (!displayed)
				std::cout << "real game speed = " << realGameSpeed << std::endl;
		
		iterationsSum = 0;
		dtSum = 0;
	}
}