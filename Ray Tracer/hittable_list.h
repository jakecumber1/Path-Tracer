#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"

#include <vector>
#include "consts_n_utils.h"
//Makes a nwew instance of type thing, returning a shared_ptr<thing>, where thing is whatever class you want (in this case object)
using std::make_shared;
//pointer to allocated type with reference counting, once shared pointers fall out of scope, reference count is decremented
//once it hits 0, the object is safely deleted
//sounds veryyyy similar to soft/hard linking in linux
//Why on earth would we use this? welp, if we have multiple of the same geometry sharing the same instance (imagine like a bunch of spheres w the same color material)
//This makes memory management significantly easier.
using std::shared_ptr;

class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(shared_ptr<hittable> object) {
		objects.push_back(object);
	}

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = ray_tmax;

		for (const auto& object : objects) {
			if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};

#endif
