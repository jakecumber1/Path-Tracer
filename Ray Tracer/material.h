#ifndef MATERIAL_H
#define MATERIAL_H
#include "hittable.h"

class material {
public:
	virtual ~material() = default;
	//returns a bool because not all materials scatter
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const {
		return false;
	}
};
//Add a class for materials which perform lambertian reflection
class lambertian : public material {
public:
	//albedo in this case just means fractional reflectance
	lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero()) {
			//if the random vector we generate is almost the same as the normal but negative
			//we will get a 0 scatter direction vector, which later on will be really really bad
			//so we'll handle it now.
			scatter_direction = rec.normal;
		}
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class metal : public material {
public:
	//fuzz will define the radius of a sphere which we will choose a point on to randomize the reflected direction
	//this gives us a "fuzzy" effect
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	color albedo;
	double fuzz;
};

#endif // !MATERIAL_H
