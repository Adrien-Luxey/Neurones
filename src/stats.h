#ifndef STATS_H
#define STATS_H

#include <vector>
#include <iostream>
#include <sstream>

#include "entity.h"
#include "utils.h"

class Stats {
	public :
		/*const static std::string averageScore(const std::vector<Entity*> entities) {
			std::stringstream ss;
			float score = 0;
			int cnt = 0;
			
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() != ENTITY) {
					score += ((Animal*) entities[i])->getScore();
					cnt++;
				}
			}
			if (cnt != 0)
				score /= (float) cnt;
			else
				score = 0;
			
			ss << "Average =\t" << score;
			return ss.str();
		}
		
		const static unsigned int highScoreInt(const std::vector<Entity*> entities) {
			unsigned int high = 0;
			
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() != ENTITY && ((Animal*) entities[i])->getScore() > high)
					high = ((Animal*) entities[i])->getScore();
			}
			
			return high;
		}
		
		const static std::string highScore(const std::vector<Entity*> entities) {
			std::stringstream ss;
			
			ss << "High score =\t" << highScoreInt(entities);
			return ss.str();
		}
		
		const static std::string totalScore(const std::vector<Entity*> entities) {
			std::stringstream ss;
			int score = 0, deads = 0;
			
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() == FOX) {
					score += ((Animal*) entities[i])->getScore();
					if (((Animal*) entities[i])->isDead())
						deads++;
				}
			}
			ss << "Fox :\t" << score << " kills, " << deads << " deaths.\n";
			
			score = 0;
			deads = 0;
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() == CHICKEN) {
					score += ((Animal*) entities[i])->getScore();
					if (((Animal*) entities[i])->isDead())
						deads++;
				}
			}
			ss << "Chicken :\t" << score << " kills, " << deads << " deaths.\n";
			
			score = 0;
			deads = 0;
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() == SNAKE) {
					score += ((Animal*) entities[i])->getScore();
					if (((Animal*) entities[i])->isDead())
						deads++;
				}
			}
			ss << "Snake :\t" << score << " kills, " << deads << " deaths.\n";
			
			score = 0;
			deads = 0;
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() == LYNX) {
					score += ((Animal*) entities[i])->getScore();
					if (((Animal*) entities[i])->isDead())
						deads++;
				}
			}
			ss << "Lynx :\t" << score << " kills, " << deads << " deaths.\n";
			
			score = 0;
			deads = 0;
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (entities[i]->type() == MONKEY) {
					score += ((Animal*) entities[i])->getScore();
					if (((Animal*) entities[i])->isDead())
						deads++;
				}
			}
			ss << "Monkeys :\t" << score << " kills, " << deads << " deaths.\n";
			
			return ss.str();
		}
		
		const static std::string printDetailledScore(const std::vector<Entity*> entities) {
			std::stringstream ss;
			
			std::vector<int> stats; // x = score; y = nb of animals with that score
			
			unsigned int hs = highScoreInt(entities);
			for (unsigned int i = 0; i <= hs; i++)
				stats.push_back(0);
			
			for (unsigned int i = 0; i < entities.size(); i++) {
				if (((Animal*) entities[i])->type() != ENTITY)
					stats[((Animal*) entities[i])->getScore()]++;
			}
			
			for (unsigned int i = 0; i < stats.size(); i++)
				if (stats[i] != 0)
					ss << i << "(" << stats[i] << ")\t";
			
			return ss.str();	
		}*/
};

#endif // STATS_H
