#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "display.h"
#include "entityManager.h"
#include "genetics.h"

class Game {
	public :
		Game();
		~Game();
		
		void exec();
		void quit() { continuer = false; }
		
		void newGeneration();
		int getGeneration() const { return generation; }
		int getIteration() const { return iteration; }
		int getIterationsPerGeneration() const { return iterationsPerGeneration; }
		const int getFps() { return fps; }
		float getGameSpeed() const { return gameSpeed; }
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
		
		//fps
		float gameSpeed;
		int fps, iteration;
		
		void update();
		
		bool gameover();
		
		void updateFps();
};

#endif // GAME_H
