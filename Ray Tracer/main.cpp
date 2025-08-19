#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {

	//Below is the graphics equivalent of "hello world"

	//image size
	int image_width = 256;
	int image_height = 256;

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
	P6 is another type of ppm header format, specifying a binary version instead of raw numbers
	this is faster but much, much harder to read and write.


	*/

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	//currently pixels rendered in rows left to right, top to bottom
	for (int j = 0; j < image_height; j++) {
		//Progress bar for particularly long renders
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			color pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                \n";
}