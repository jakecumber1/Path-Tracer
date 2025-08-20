#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

//hardcode a sphere into our image for now
//returning the normal of our sphere hit point
double hit_sphere(const point3& center, double radius, const ray& r) {
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
	auto discriminant = h*h - a * c;
	if (discriminant < 0) {
		//no real solutions, ray missed the sphere
		return -1.0;
	}
	else {
		//return the nearest "t" where the ray intersects the sphere
		return (h - std::sqrt(discriminant)) / a;
	}
}


color ray_color(const ray& r) {
	//determines if the ray hits the sphere
	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	//if so compute the surface normal at the hit point
	if (t > 0.0) {
		//hit point minus center, normalized
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		//here we map [-1,1] range of the normal to [0, 1] color range
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	//no hit, draw background gradient
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main() {

	//Image

	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	
	//make sure image height is at least 1
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	//Camera

	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	//Calculate vectors across horizontal and down vertical viewport edges
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	//Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	//calculate location of upper left pixel
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


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
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);

		}
	}
	std::clog << "\rDone.                \n";
}