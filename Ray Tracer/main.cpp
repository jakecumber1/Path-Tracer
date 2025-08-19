#include <iostream>

int main() {

	//Below is the graphics equivalent of "hello world"

	//image size
	int image_width = 256;
	int image_height = 256;

	//Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	//currently pixels rendered in rows left to right, top to bottom
	for (int j = 0; j < image_height; j++) {
		for (int i = 0; i < image_width; i++) {
			//r, b, g range from 0.0 to 1.0
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_width - 1);
			auto b = 0.0;
			//then rgb are scaled to an int value between 0 and 255
			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}