#include "game.h"


Game::Game() 
  : display(this), continuer(true), pause(false), generation(1),
	iterationsPerGeneration(CFG->readInt("IterationsPerGeneration")),
	defaultGameSpeed(CFG->readInt("DefaultGameSpeed")), minimumFps(CFG->readInt("MinimumFps")),
	gameSpeedRatio(1), fps(0), ups(0), iterations(0), updatesCount(0), displaysCount(0) {	
	
}

Game::~Game() {}

/* Game loop basée sur celle de DeWITTERS, à cette adresse :
 * http://www.koonsolo.com/news/dewitters-gameloop/
 * 
 * Cela permet de controler la vitesse d'execution de l'update, de ne pas dépendre de l'affichage, et de le fluidifier
 */
void Game::exec() {
	clock.restart();  
	oneSecondClock.restart();
	
	while (continuer) {
		display.events();
		
		sf::Time updateDeltaTime = sf::seconds(1) / (gameSpeedRatio * defaultGameSpeed);
		
		if (!pause) {
			maxDisplayTick = clock.getElapsedTime() + sf::seconds(1) / (float) minimumFps;
			
			while( clock.getElapsedTime() > nextUpdateTick && clock.getElapsedTime() < maxDisplayTick ) {
				update();
				
				updatesCount++;
				nextUpdateTick += updateDeltaTime;
			}
			
			updateFps();
			
			sf::Time interpolation = clock.getElapsedTime() + updateDeltaTime - nextUpdateTick;
			
			display.update(manager, interpolation.asSeconds() / updateDeltaTime.asSeconds());
			displaysCount++;
		}
	}
}

void Game::togglePause() {
	pause = !pause;
}

void Game::increaseGameSpeed() {
	gameSpeedRatio = (gameSpeedRatio >= 1) ? gameSpeedRatio + 1 : gameSpeedRatio*2;
}
void Game::decreaseGameSpeed() {
	gameSpeedRatio = (gameSpeedRatio > 1) ? gameSpeedRatio - 1 : gameSpeedRatio/2;
}

void Game::newGeneration() {
	genetics.evolve(manager);
	manager.init();
	
	iterations = 0;
	
	std::cout << "Génération #" << generation++ << " lasted " << clock.restart().asSeconds() << " seconds" << std::endl;
	nextUpdateTick = sf::Time::Zero;
	maxDisplayTick = sf::Time::Zero;
}

void Game::update() {
	if (gameover())
		newGeneration();
	
	manager.update();
	
	iterations++;
}

bool Game::gameover() {	
	if (iterations >= iterationsPerGeneration || manager.gameover())
		return true;
	
	return false;
}

void Game::updateFps() {
	if (oneSecondClock.getElapsedTime() >= sf::seconds(1)) {
		float dt = oneSecondClock.restart().asSeconds();
		fps = displaysCount / dt;
		ups = updatesCount / dt;
		
		displaysCount = 0;
		updatesCount = 0;
	}
}