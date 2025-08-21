#include "consts_n_utils.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {


	//World
	hittable_list world;

	//define material types
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
	//ground is just a giant sphere pushed down and given ground material
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	//sphere showing off lambertian material
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
	//sphere showing off metal material 1 (left)
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	//sphere showing off metal material 2 (right)
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	//camera
	camera cam;
	//set our ratio and image width
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	//antialiasing effect with random point selection in a square.
	cam.samples_per_pixel = 100;
	//set max depth to keep ray_color from recursing too far
	cam.max_depth = 50;

	cam.render(world);

}