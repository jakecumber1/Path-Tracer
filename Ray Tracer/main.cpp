#include "consts_n_utils.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"


int main() {


	//World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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