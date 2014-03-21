#ifndef UTILS_H
#define	UTILS_H

#include <cmath>
#include <random>
#include <iostream>

#include "config_parser.h"

/// Generator used by every c++11 random function (normal, uniform)
static std::default_random_engine generator;

/// shortcut for calling the configParser singleton
#define CFG		ConfigParser::get()

/// @brief PI definition in stl depends on installation, so let's define my own
#define PI				3.14159f

/// @brief enumeration to give sense to all the species (used for colouring)
enum { CHICKEN, FOX, SNAKE, LYNX, MONKEY, FISH, TYPES_CNT };

/// @brief A structure reprensenting standard 2D vectors of int, with constructor
typedef struct Vect2i {
	int x;
	int y;

	Vect2i(int _x = 0, int _y = 0) : x(_x), y(_y) {}
}Vect2i;

/**
 * @brief Utility function to return the sign of a value
 * @param val	The value 
 * @return	The sign
 */
template <typename T> static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif	/* UTILS_H */

