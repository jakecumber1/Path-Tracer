#ifndef COLOR_H
#define COLOR_H
#include "interval.h"
#include "vec3.h"


using color = vec3;

//converts linear images to gamma space
inline double linear_to_gamma(double linear_component) {
	if (linear_component > 0) {
		return std::sqrt(linear_component);
	}
	return 0;
}



//write's color to output
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//transform from linear rgb to gamma transform for gamma 2
	/*We need to do this because our computer assumes the image
	has been gamma corrected when writing bytes to our image file
	if we don't gamma correct it our image is in "linear space" as
	opposed to "gamma space" this results in a much darker image than intended
	for our use case.*/
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	//translate from [0, 1] to [0, 255]
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	//write out color components to outstream
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif