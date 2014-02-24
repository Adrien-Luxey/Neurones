#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>

#include "display.h"
#include "entityManager.h"
#include "genetics.h"

class Game {
	public :
		Game();
		~Game();
		
		void exec();
		void quit() { continuer = false; }
		
		int getGeneration() const { return generation; }
		int getIterations() const { return iterations; }
		int getIterationsPerGeneration() const { return iterationsPerGeneration; }
		float getGameSpeed() const { return gameSpeed; }
		float getRealGameSpeed() const { return realGameSpeed; }
		void togglePause();
		void increaseGameSpeed();
		void decreaseGameSpeed();
		
		// getters
		void toggleDisplayed() { displayed = !displayed; }
		
	private :		
		Display display;
		EntityManager manager;
		Genetics genetics;
		
		bool continuer, pause, displayed;
		int generation;
		
		const int iterationsPerGeneration;
		
		// gestion du temps
		float gameSpeed, dt, dtSum, realGameSpeed;
		int iterations, loopIterations, iterationsSum;
		
		void newGeneration();
		void update();
		bool gameover();
		void updateGameSpeed();
};

#endif // GAME_H
