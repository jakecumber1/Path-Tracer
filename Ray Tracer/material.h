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

/*Some helpful definitions for defining materials
mainly for understanding dielectrics when we implement that material:
-reflected ray = ray hits a surface and bounces off in a new direction
-refracted ray = ray bends when it transforms from a material's surroundings into the material itself
refracted rays are why pencils look bent when placed partially in water as an example
-refraction index = the amount a refracted ray bends (glass is 1.5-1.7, diamond 2.4, air 1.000293)
For transparent materials within transparent materials, the refraction can be described with a relative refraction index
-relative refraction index = refractive index of the embedded material / refractive index of surrounding material
for example a glass ball under water would have a relative reflective index of 1.125 (1.5/1.333)
-total internal reflection: when traveling from a denser medium to a less dense one at the right angle, all the rays are reflected bac into the denser medium
one example to book gives is water to air, if you're looking straight up, you can see through fine, but if you are near the surface looking sideways,
the boundary acts like a mirror*/



class dielectric : public material {
public:
	dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		//for glass it absorbs nothing, so attenuation is always 1
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;
		//we are passed the critical angle, so we should reflect instead of refract
		if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, ri);
		}

		scattered = ray(rec.p, direction);
		return true;
	}
private:
	//refracting index in vacuum or air, or ratio of materials refractive index
	//over refactive index of enclosing material
	double refraction_index;
	//Uses Schlick's approximation to determine reflectivity based on angle
	static double reflectance(double cosine, double refraction_index) {
		//schlick's approximation is a really cheap way to deterimine reflectance.
		//read more about the formula here: https://en.wikipedia.org/wiki/Schlick%27s_approximation
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};

#endif // !MATERIAL_H
