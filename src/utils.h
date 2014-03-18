/* 
 * File:   utils.h
 * Author: adrien
 *
 * Created on 5 décembre 2013, 01:07
 */

#ifndef UTILS_H
#define	UTILS_H

#define PI				3.14159f

#include <cmath>
#include <random>
#include <iostream>
static std::default_random_engine generator;

#include "config_parser.h"
#define CFG		ConfigParser::get()

// enumeration to give sense to all the species (used for colouring))
enum { CHICKEN, FOX, SNAKE, LYNX, MONKEY, FISH, TYPES_CNT };

// A structure reprensenting standard 2D vectors of int, with constructor
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

