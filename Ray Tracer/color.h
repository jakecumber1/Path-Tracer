#ifndef COLOR_H
#define COLOR_H
#include "interval.h"
#include "vec3.h"


using color = vec3;

//write's color to output
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//translate from [0, 1] to [0, 255]
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	//write out color components to outstream
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif