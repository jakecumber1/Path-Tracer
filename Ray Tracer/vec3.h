#ifndef VEC3_H
#define VEC3_H

#include "consts_n_utils.h"
class vec3 {
public:
	double e[3];

	vec3() : e{ 0,0,0 } {};
	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	//Self subtraction operator returns all elements in the vec times negative 1
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	//x, y, z element access operator, read only
	double operator[](int i) const { return e[i]; }
	//x, y, z element access, but returns reference (i.e allows assignment).
	double& operator[](int i) { return e[i]; }

	vec3& operator+= (const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(double t) {
		return *this *= 1 / t;
	}
	double length() const {
		return std::sqrt(length_squared());
	}
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}


};

//point3 is an alias for vec3, but will be useful for geometry clarity
using point3 = vec3;

//vector utility functions


//inline is a keyword i don't use very often
//basically tells the compiler, "hey replace function calls to this function with what is contained within the function
//converting something like y = square(3); to y = 3 * 3;

//Prints out our vector to console.
inline std::ostream& operator<< (std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
//vector addition
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}


//vector subtraction
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
//vector element multiplication
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
//scalar multiplication
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
// v * t is not the same as t * v, but should be

//scalar multiplication
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

//scalar division.
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}
//don't need to handle the opposite ordering because... what would that even look like?

//vector dot product.
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

//vector cross product
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
//Convert the vector into a unit vector.
inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

#endif