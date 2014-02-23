#include "game.h"


Game::Game() 
  : display(this), continuer(true), pause(false), displayed(true), generation(1), epocDuration(CFG->readInt("EpocDuration")),
	dt(0), elapsedTime(0), dtSum(0), frames(0), gameSpeed(1), fps(0), loopsSinceLastDisplay(0) {	
	
}

Game::~Game() {}

void Game::exec() {
	while (continuer) {
		dt = display.getElapsedTime();
		display.events();
		
		if (!pause) {
			loopsSinceLastDisplay++;
			
			update();
			
			if (gameSpeed < 1 || loopsSinceLastDisplay >=  gameSpeed) {
				if (displayed)
					display.update(manager);
				
				loopsSinceLastDisplay = 0;
			}
		}
	}
}

void Game::update() {
	updateFps();
	
	if (gameover())
		newGeneration();
	
	// dt * gameSpeed => le manager pense que le frameRate est toujours le même qu'à gamespeed = 1
	// Sinon la vitesse des mobiles ne serait pas accélérée
	manager.update(dt * gameSpeed);
}

void Game::newGeneration() {
	genetics.evolve(manager);
	manager.init();
	elapsedTime = 0;
	
	std::cout << "Génération #" << generation++ << std::endl;
}

bool Game::gameover() {
	if (elapsedTime > epocDuration)
		return true;
	
	if (manager.gameover())
		return true;
	
	return false;
}

void Game::togglePause() {
	pause = !pause;
}

void Game::updateFps() {
	elapsedTime += dt * gameSpeed;	
	dtSum += dt;
	frames++;
	
	if (dtSum > 1) {
		fps = frames/dtSum;
		
		if (!displayed) {
			std::chrono::microseconds sleepDuration((int) (1000000 * (dtSum - 1)));
			std::cout << "FPS : " << fps << ", sleepTime : " << sleepDuration.count() << ", gameSpeed : " << gameSpeed <<  std::endl;
			std::this_thread::sleep_for(sleepDuration);
		}
		
		frames = 0;
		dtSum = 0;
	}
}

void Game::increaseGameSpeed() {
	gameSpeed = (gameSpeed >= 1) ? gameSpeed + 1 : gameSpeed*2;
}
void Game::decreaseGameSpeed() {
	gameSpeed = (gameSpeed > 1) ? gameSpeed - 1 : gameSpeed/2;
}
