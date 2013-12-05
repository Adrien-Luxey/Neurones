#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include "display.h"
#include "entityManager.h"
#include "genetics.h"
#include "stats.h"

class Game {
	public :
		Game();
		~Game();
		
		void exec();
		void quit() { continuer = false; }
		
		void newGeneration();
		const int getGeneration() { return generation; }
		const float getElapsedTime() { return elapsedTime; }
		const int getFps() { return fps; }
		void togglePause();
		
	private :		
		Display display;
		EntityManager manager;
		Genetics genetics;
		
		bool continuer, pause;
		int generation, epocDuration;
		
		//fps
		float dt, elapsedTime, dtSum, frames;
		int fps;
		
		void update();
		
		bool gameover();
		
		void updateFps();
};

#endif // GAME_H
