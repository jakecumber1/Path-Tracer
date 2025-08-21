#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "consts_n_utils.h"

using color = vec3;

//write's color to output
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//translate from 0-1 to 0-255
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	//write out color components to outstream
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif