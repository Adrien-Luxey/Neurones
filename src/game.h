#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>

#include <SFML/System.hpp>

#include "display.h"
#include "entity_manager.h"
#include "genetics.h"

/**
 * @brief General class dispatching the game jobs to other objects. Only SFML user with Display
 * 
 */
class Game {
	public :
		Game();
		~Game();
	
		/**
		 * @brief Game loop
         */
		void loop();
		
		void quit() { continuer = false; }
		void increaseGameSpeed();
		void decreaseGameSpeed();
		
		// getters
		int getGeneration() const { return generation; }
		int getIterations() const { return iterations; }
		int getIterationsPerGeneration() const { return ITERATIONS_PER_GENERATION; }
		float getGameSpeedRatio() const { return gameSpeedRatio; }
		float getFps() const { return fps; }
		float getUps() const { return ups; }
		
	private :
		/**
		 * @brief calls the genetic algorithm and resets everything for the next generation
         */
		void newGeneration();
		/**
		 * @brief Game update : calls the manager and checks for the end of the generation
         */
		void update();
		/**
		 * @brief Updates fps & ups every second for user information
         */
		void updateFps();
		
		Display display;
		EntityManager manager;
		Genetics genetics;
		
		bool continuer;
		int generation;
		
		const int ITERATIONS_PER_GENERATION, DEFAULT_GAME_SPEED, MINIMUM_FPS;
		
		// time handling attributes
		sf::Clock clock, oneSecondClock;
		sf::Time nextUpdateTick, maxDisplayTick;
		float gameSpeedRatio;
		float fps, ups /* updates per second */;
		int iterations, updatesCount, displaysCount;
};

#endif // GAME_H
