#pragma once

#include <vector>

std::vector<std::vector<int>> mandelbrot(
	std::size_t width, std::size_t height,
	double realmax, double realmin,
	double imagmax, double imagmin,
	int maxiter);

std::vector<std::vector<int>> mandelbrot_avx(
	std::size_t width, std::size_t height,
	double realmax, double realmin,
	double imagmax, double imagmin,
	int maxiter);
