#include "consts_n_utils.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {


	//World
	hittable_list world;
	//grey colored ground
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	//sphere representing the ground
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	//For loop to populate world with spheres
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			//generate a random double to pick which material to use
			auto choose_mat = random_double();
			//place the sphere within a certain radius of a and b iteration
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;
				if (choose_mat < 0.8) {
					//create diffuse material, approx 80% of spheres will be lambertian
					//randomly generate sphere color
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					//create metal material, approx 15% of our spheres will be metal
					//randomly generate our metal's color and fuzziness
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					//finally make a glass material, approximately 5% of the spheres will be glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	//make 3 bigger spheres to show off each type of material
	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	//camera
	camera cam;
	//set our ratio and image width
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	//antialiasing effect with random point selection in a square.
	cam.samples_per_pixel = 500;

	//set maximum recursion for ray_color
	cam.max_depth = 50;

	//set camera position and angle
	cam.vfov = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.render(world);

}