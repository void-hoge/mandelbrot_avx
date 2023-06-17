#include "bmp.hpp"

bmp::vec3 hsv2rgb(int h, double s, double v) {
	double h_ = (double)(h%60)/60;
	uint8_t a = 255*v;
 	uint8_t b = 255*v*(1-s);
	uint8_t c = 255*v*(1-s*h_);
	uint8_t d = 255*v*(1-s*(1-h_));
	if (s == 0) {
		return bmp::vec3(a,a,a);
	}else if (0 <= h && h < 60) {
		return bmp::vec3(a,d,b);
	}else if (60 <= h && h < 120) {
		return bmp::vec3(c,a,b);
	}else if (120 <= h && h < 180) {
		return bmp::vec3(b,a,d);
	}else if (180 <= h && h < 240) {
		return bmp::vec3(b,c,a);
	}else if (240 <= h && h < 300) {
		return bmp::vec3(d,b,a);
	}else /*if (300 <= h && h < 360)*/ {
		return bmp::vec3(a,b,c);
	}
}

std::vector<std::vector<bmp::vec3>> countmap2bitmap(
	std::vector<std::vector<int>>& countmap, int maxiter) {
	const std::size_t height = countmap.size();
	const std::size_t width = countmap[0].size();
	auto bitmap = std::vector<std::vector<bmp::vec3>>(
		height, std::vector<bmp::vec3>(
			width, bmp::vec3()));
	for (std::size_t i = 0; i < height; i++) {
		for (std::size_t j = 0; j < width; j++) {
			auto num = countmap[i][j];
			if (num < maxiter) {
				bitmap[i][j] = hsv2rgb((num*2)%360, 1.0f, 1.0f);
			}
		}
	}
	return bitmap;
}
