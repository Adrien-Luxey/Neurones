#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

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
		float getElapsedTime() const { return elapsedTime; }
		int getEpocDuration() const { return epocDuration; }
		const int getFps() { return fps; }
		void togglePause();
		
	private :		
		Display display;
		EntityManager manager;
		Genetics genetics;
		
		bool continuer, pause;
		int generation;
		
		const int epocDuration;
		
		//fps
		float dt, elapsedTime, dtSum, frames;
		int fps;
		
		void update();
		
		bool gameover();
		
		void updateFps();
};

#endif // GAME_H
