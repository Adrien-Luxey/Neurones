/**
 * /file game.h
 * /author Adrien Luxey
 * /brief Main class, containing the game loop, game variables, and calls to the other classes
 * 
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>

#include <SFML/System.hpp>

#include "display.h"
#include "entityManager.h"
#include "genetics.h"

class Game {
	public :
		Game();
		~Game();
		
		void exec();
		void quit() { continuer = false; }
		
		// getters and stuff
		int getGeneration() const { return generation; }
		int getIterations() const { return iterations; }
		int getIterationsPerGeneration() const { return iterationsPerGeneration; }
		float getGameSpeedRatio() const { return gameSpeedRatio; }
		float getFps() const { return fps; }
		float getUps() const { return ups; }
		void togglePause();
		void increaseGameSpeed();
		void decreaseGameSpeed();
		
	private :		
		Display display;
		EntityManager manager;
		Genetics genetics;
		
		bool continuer, pause;
		int generation;
		
		const int iterationsPerGeneration, defaultGameSpeed, minimumFps;
		
		// gestion du temps
		sf::Clock clock, oneSecondClock;
		sf::Time nextUpdateTick, maxDisplayTick;
		float gameSpeedRatio;
		float fps, ups /* updates per second */;
		int iterations, updatesCount, displaysCount;
		
		void newGeneration();
		void update();
		bool gameover();
		void updateFps();
};

#endif // GAME_H
