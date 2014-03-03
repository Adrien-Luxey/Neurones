#include "game.h"


Game::Game() 
  : display(this), continuer(true), generation(1),
	ITERATIONS_PER_GENERATION(CFG->readInt("IterationsPerGeneration")),
	DEFAULT_GAME_SPEED(CFG->readInt("DefaultGameSpeed")), MINIMUM_FPS(CFG->readInt("MinimumFps")),
	gameSpeedRatio(1), fps(0), ups(0), iterations(0), updatesCount(0), displaysCount(0) {	
	
}

Game::~Game() {}

/* Game loop basée sur celle de DeWITTERS, à cette adresse :
 * http://www.koonsolo.com/news/dewitters-gameloop/
 * 
 * Cela permet de controler la vitesse d'execution de l'update, de ne pas dépendre de l'affichage, et de le fluidifier
 */
void Game::loop() {
	clock.restart();  
	oneSecondClock.restart();
	
	while (continuer) {
		display.events();
		
		sf::Time updateDeltaTime = sf::seconds(1) / (gameSpeedRatio * DEFAULT_GAME_SPEED);
		
		maxDisplayTick = clock.getElapsedTime() + sf::seconds(1) / (float) MINIMUM_FPS;

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
	
	sf::Time generationDuration = clock.restart();
	std::cout << "Génération #" << generation++ << " lasted " << generationDuration.asSeconds() << " seconds" << std::endl;
	nextUpdateTick -= generationDuration;
	maxDisplayTick -= generationDuration;
}

void Game::update() {
	if (iterations >= ITERATIONS_PER_GENERATION)
		newGeneration();
	
	manager.update();
	
	iterations++;
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