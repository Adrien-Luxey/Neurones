#ifndef UTILS_H
#define UTILS_H

#define PI				3.14159f

#define NETWORK_INPUTS	11
#define NETWORK_OUTPUTS	2

#include <cmath>
#include <random>
static std::default_random_engine generator;

#include "configParser.h"
#define CFG		ConfigParser::get()

enum { ENTITY, CHICKEN, FOX, SNAKE, LYNX, MONKEY, TYPES_CNT };

typedef struct Vect2i {
	int x;
	int y;
}Vect2i;

#endif // UTILS_H
