#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include "display.h"
#include "entity.h"
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
		void update();
		
		bool gameover();
		
		Display display;
		Genetics genetics;
		std::vector<Entity*> entities;
		bool continuer, pause;
		int generation;
		float dt, elapsedTime, dtSum, frames;
		int fps;
};

#endif // GAME_H
