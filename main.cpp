#include "mandelbrot.hpp"
#include "bmp.hpp"
#include "util.hpp"
#include <string>
#include <iostream>
#include <chrono>

int main() {
	const std::string re_str = "0.36024044343761436323612524444954530";
	const std::string im_str = "-0.64131306106480317486037501517930206";
	const std::string zoom = "0.000000000001";
	auto width = 2560;
	auto height = 1600;
	auto re = std::stod(re_str);
	auto im = std::stod(im_str);
	auto wrange = std::stod(zoom);
	auto hrange = std::stod(zoom)*((double)height/width);
	auto maxiter = 10000000;
	std::cout << "re:      " << re << std::endl
			  << "im:      " << im << std::endl
			  << "wrange:  " << wrange << std::endl
			  << "hrange:  " << hrange << std::endl
			  << "maxiter: " << maxiter << std::endl;
	std::cout << "Generating mandelbrot set image..." << std::endl;
	auto start = std::chrono::system_clock::now();
	auto countmap = mandelbrot_avx(
		width, height,
		re+wrange/2, re-wrange/2,
		im+hrange/2, im-hrange/2,
		maxiter);
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	std::cout << "Elapsed " << elapsed << " ms" << std::endl;
	std::cout << "Converting divergence count map to bitmap image..." << std::endl;
	auto bitmap = countmap2bitmap(countmap, maxiter);
	auto image = bmp::bmp_image(bitmap);
	image.save("test.bmp");
	return 0;
}
