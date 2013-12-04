#ifndef UTILS_H
#define UTILS_H

#define PI				3.14159f

#define NETWORK_INPUTS	11
#define NETWORK_OUTPUTS	2

#include <cmath>
#include <random>

#include "configParser.h"
#define CFG		ConfigParser::get()


static std::default_random_engine generator;

enum { ENTITY, CHICKEN, FOX, SNAKE, LYNX, MONKEY, TYPES_CNT };

typedef struct Vect2i {
	int x;
	int y;

	// Constructeurs
	Vect2i() : x(0), y(0) {}
	Vect2i(int _x, int _y) : x(_x), y(_y) {}
}Vect2i;

#endif // UTILS_H
