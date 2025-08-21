#ifndef CONSTS_N_UTILS_H
#define CONSTS_N_UTILS_H
//file for all our constants and useful util functions

#include <cmath>
//we'll opt for implementing random doubles using the C functions instead of the C++ <random> ones
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

//C++ std::usings

using std::make_shared;
using std::shared_ptr;

//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415826535897932385;

//utility funcs

//converts degrees to radians
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

//returns a random double between [0, 1).
inline double random_double() {
	return std::rand() / (RAND_MAX + 1.0);
}
//returns a random double in the interval [min, max)
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}


//common headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif
