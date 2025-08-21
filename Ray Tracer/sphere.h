#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
	sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {};

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
		//oc = vector from ray origina to sphere center
		vec3 oc = center - r.origin();
		//coefficients for quadratic formula
		//remember the quadratic equation we are solving is at^2 + bt + c = 0
		//length of squared direction
		auto a = r.direction().length_squared();
		//sub b for -2h, allowing us to simplify the discriminant to
		//4(h^2 - ac)
		//allowing us to pull the 4 our and end up with 2h - 2 * sqrt(h^2 - ac) / 2a
		//which simplifies to
		//h - sqrt(h^2 - ac) / a
		//b = 2(D * oc), so let's simplify it to h = D * oc, and work with h directly.
		auto h = dot(r.direction(), oc);
		auto c = oc.length_squared() - radius * radius;
		//b^2 - 4ac part of quadratic equation
		auto discriminant = h * h - a * c;
		if (discriminant < 0) {
			//no real solutions, ray missed the sphere
			return false;
		}
		//find the nearest root which lies in the acceptable range
		auto sqrtd = std::sqrt(discriminant);
		//+ or - part of the quadratic form
		auto root = (h - sqrtd) / a;
		if (root <= ray_tmin || ray_tmax <= root) {
			root = (h + sqrtd) / a;
			if (root <= ray_tmin || ray_tmax <= root) {
				return false;
			}
		}
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		//is our ray coming from inside or outside? and set normal accordingly
		rec.set_face_normal(r, outward_normal);
		return true;
	}
private:
	point3 center;
	double radius;
};

#endif