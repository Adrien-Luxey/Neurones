/* 
 * File:   utils.h
 * Author: adrien
 *
 * Created on 5 décembre 2013, 01:07
 */

#ifndef UTILS_H
#define	UTILS_H

#define PI				3.14159f

#define NETWORK_INPUTS	8
#define NETWORK_OUTPUTS	2

#include <cmath>
#include <random>
static std::default_random_engine generator;

#include "configParser.h"
#define CFG		ConfigParser::get()
const static int worldSize = CFG->readInt("WorldSize");

enum { ENTITY, CHICKEN, FOX, SNAKE, LYNX, MONKEY, FISH, TYPES_CNT };

typedef struct Vect2i {
	int x;
	int y;
}Vect2i;


#endif	/* UTILS_H */

