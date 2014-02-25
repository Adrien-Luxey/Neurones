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
#define NETWORK_OUTPUTS	3

#include <cmath>
#include <random>
#include <iostream>
static std::default_random_engine generator;

#include "configParser.h"
#define CFG		ConfigParser::get()

enum { CHICKEN, FOX, SNAKE, LYNX, MONKEY, FISH, TYPES_CNT };

typedef struct Vect2i {
	int x;
	int y;
	
	Vect2i() : x(0), y(0) {}
	Vect2i(int _x, int _y) : x(_x), y(_y) {}
}Vect2i;

template <typename T> static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif	/* UTILS_H */

