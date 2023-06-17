#pragma once
#include "bmp.hpp"

bmp::vec3 hsv2rgb(int h, double s, double v);

std::vector<std::vector<bmp::vec3>> countmap2bitmap(
	std::vector<std::vector<int>>& countmap, int maxiter);
