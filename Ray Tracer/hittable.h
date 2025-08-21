#ifndef HITTABLE_H
#define HITTABLE_H

//Class for objects which can be hit by rays, not a table of hits!
#include "consts_n_utils.h"

class hit_record {
public:
	point3 p;
	vec3 normal;
	double t;
	//decision time, do we want normals to always point outwards or always point against the ray?
	//if normals always point outwards, we can check if a ray is inside or outside the sphere by checking it's direction against the normals
	//if their dot product is negative, outside (ray pointing towards the sphere from outside, normal points back at it), if it's positive, the ray originates from
	// inside (ray from the inside points the same direction as the normal)
	// however, we have another route: normals always point back at the rays we need to store that information instead
	//with something like bool front_face
	//what mainly drives what route we'll take is dependant on whether we want to deterimine the side at geometry intersection (the latter route), or coloring (the former route)
	//We have many material types we plan to use, so geometry is preferable, as opposed to having every material shader check whenever it is swapped out.
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		//sets hit record normal vector, outward_normal is assumed to be of unit length
		//we have a front_face if ray and normal are pointing in opposite directions
		front_face = dot(r.direction(), outward_normal) < 0;
		//the normal is changed to point back at the ray.
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;
	//a hit is only valid if t is between tmin and tmax!
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif
