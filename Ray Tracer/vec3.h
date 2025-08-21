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
	//determines if vector is close to 0 in all dimensions
	bool near_zero() const {
		//returns true if vector is close to 0 in all dimensions
		auto s = 1e-8;
		//fabs = floating point absolute value
		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < 1) && (std::fabs(e[2]) < 1);
	}

	//returns a vector of random double elements
	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	//return a vector of random elements that fall within the given interval.
	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
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
//return a unit vector of random elements
inline vec3 random_unit_vector() {
	while (true) {
		auto p = vec3::random(-1, 1); // pick a point in the cube [-1, 1]^3
		auto lensq = p.length_squared(); // compute squared length of that point vector
		if (1e-160 < lensq && lensq <= 1) { //if the point lies in the unit sphere, but if the length is too small we will get an inf vector when we normalize.
			return p / sqrt(lensq); //normalize p to make it a unit vector, otherwise we'd be sampling in the sphere instead of the surface.
		}
		//for more info look up lambertuan scattering
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) { //this means the vector is in the same hemisphere as the normal, i.e we want them pointing in or out the same way.
		return on_unit_sphere;
	} else {  //less than or equal to 0, the vector isn't in the correct hemisphere
		//just invert it and we have a valid vector
		return -on_unit_sphere;
	} 
}
//Computes the reflection ray off a surface
inline vec3 reflect(const vec3& v, const vec3& n) {
	/*v is the ray going towards the surface
	n is the surface normal, a unit vector pointing out of the surface
	remove the component of the vector in the direction of the normal, then flip it*/
	return v - 2 * dot(v, n) * n;
}

//Computes refraction of a ray passing through a surface.
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	//look up snell's law to learn more about refraction if you want
	auto cos_theta = std::fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

#endif