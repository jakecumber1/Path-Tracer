#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
public:
	double aspect_ratio = 1.0; //ratio of width over height
	int image_width = 100; //rendered image width in pixel count
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
				//the pixels center is just (0, 0) + the horizontal change between pixels times i and the vertical change times j
				auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				//could be made into a unit vector, but for now this is simpler and faster.
				auto ray_direction = pixel_center - center;
				ray r(center, ray_direction);

				color pixel_color = ray_color(r, world);
				write_color(std::cout, pixel_color);

			}
		}
		std::clog << "\rDone.                \n";
	}
private:

	int image_height; //rendered image height
	point3 center; //camera center
	point3 pixel00_loc; //location of pixel 0,0
	vec3 pixel_delta_u; //horizontal difference between this pixel and the one on the right
	vec3 pixel_delta_v; //vertical difference between this pixel and the one below

	void initialize() {
		//make sure image height is at least 1
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = point3(0, 0, 0);

		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		//Calculate vectors across horizontal and down vertical viewport edges
		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		//Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		//calculate location of upper left pixel
		auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	color ray_color(const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			return 0.5 * (rec.normal + color(1, 1, 1));
		}
		//no hit, draw background gradient
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif
