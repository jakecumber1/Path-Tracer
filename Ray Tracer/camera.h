#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
	double aspect_ratio = 1.0; //ratio of width over height
	int image_width = 100; //rendered image width in pixel count
	int samples_per_pixel = 10; //count of random samples per pixel
	int max_depth = 10; //max number of ray bounces, used to save guard ray_color from exploding our stack w recursion

	double vfov = 90; //vertical field of view

	//define a lookfrom, lookat, and vup for camera positioning
	point3 lookfrom = point3(0, 0, 0); //point camera looks from
	point3 lookat = point3(0, 0, -1); // point camera looks at
	vec3 vup = vec3(0, 1, 0); //camera-relative "up" direction

	//set up variables for our depth of field
	double defocus_angle = 0; //variation angle of rays through each pixel
	double focus_dist = 10; // distance from camera lookfrom point to "plane of perfect focus"


	//renders an ppm image in P3 format.
	void render(const hittable& world) {
		initialize();

		//Render

		/*
		PPM format is structured like this:
		P3
		image_width image_height
		max_color_value
		r g b r g b r g b... image_width times
		r g b r g b r g b...
		...
		image_height times

		----

		P3 specifies we're making an ascii ppm, which says pixel values
		are seperated by whitespace (space, tab, newline...)
		P6 is another type of ppm header format, specifying a raw  binary version instead of ascii characters
		this is faster but much, much harder to read and write.


		*/

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		//currently pixels rendered in rows left to right, top to bottom
		for (int j = 0; j < image_height; j++) {
			//Progress bar for particularly long renders
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);

			}
		}
		std::clog << "\rDone.                \n";
	}
private:

	int image_height; //rendered image height
	double pixel_samples_scale; //color scale factor for a sum of pixel samlpes
	point3 center; //camera center
	point3 pixel00_loc; //location of pixel 0,0
	vec3 pixel_delta_u; //horizontal difference between this pixel and the one on the right
	vec3 pixel_delta_v; //vertical difference between this pixel and the one below
	vec3 u, v, w; // camera frame basis vectors
	vec3 defocus_disk_u; //defocus disk for horizontal radius
	vec3 defocus_disk_v; //defocus disk for vertical radius

	void initialize() {
		//make sure image height is at least 1
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = lookfrom;

		//determine view point dimensions
		auto theta = degrees_to_radians(vfov);
		auto h = std::tan(theta / 2);
		auto viewport_height = 2.0 * h * focus_dist;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		//calculate u, v, w unit basis vectors for the camera coordinate fram
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);



		//Calculate vectors across horizontal and down vertical viewport edges
		vec3 viewport_u = viewport_width * u; //vector across viewpoint horizontal edge
		vec3 viewport_v = viewport_height * -v; //vector across viewpoint vertical edge.

		//Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		//calculate location of upper left pixel
		auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		//calculate camera defocus basis vectors
		auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}
	//constructs a ray originating from camera origin and directed at random sample point around pixel[i,j]
	ray get_ray(int i, int j) const {
		//construct a camera ray originating from defocus disk and directed at a randomly sampled point
		//around pixel location i, j.
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}
	//return vector to random point in a [-.5,-.5]-[+.5,+.5] unit square.
	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}
	//returns random point in the camera defocus disk.
	point3 defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);

	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		//hit the ray bounce limit, so no more light should be gathered
		if (depth <= 0) {
			return color(0, 0, 0);
		}
		hit_record rec;
		//ignore hits below 0.001 to account for shadow acne problem
		if (world.hit(r, interval(0.001, infinity), rec)) {
			//account for material type and how the ray should behave when coming in contact with the surface
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			return color(0, 0, 0);
		}
		//no hit, draw background gradient
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif
