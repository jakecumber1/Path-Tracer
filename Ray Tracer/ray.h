#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
	//Constructor
	ray() {}
	//remember, point3 was defined in vec3 as an alias, and is for location information

	//Constructor
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	//origin and direction return immutable references, callers can just use the reference of make a mutable copy.

	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }

	point3 at(double t) { orig + t * dir; }


private:
	point3 orig;
	vec3 dir;
};

#endif