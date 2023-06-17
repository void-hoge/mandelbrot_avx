#include <vector>
#include <iostream>

#include <immintrin.h>

using ll = long long;

std::size_t mandelbrot_kernel(
	double real, double imag,
	int maxiter) {
	double x = real;
	double y = imag;
	for (std::size_t i = 0; i < maxiter; i++) {
		double t;
		t = x*x - y*y + real;
		y = x*y*2.0 + imag;
		x = t;
		if ((x*x + y*y) > 4.0) {
			return i;
		}
	}
	return maxiter;
}

std::vector<std::vector<int>> mandelbrot(
	std::size_t width, std::size_t height,
	double realmax, double realmin,
	double imagmax, double imagmin,
	int maxiter) {
	double dr = (realmax - realmin)/width;
	double di = (imagmax - imagmin)/height;
	auto grid = std::vector<std::vector<int>>(
		height, std::vector<int>(width, 0));
#pragma omp parallel for collapse(2)
	for (std::size_t i = 0; i < height; i++) {
		for (std::size_t j = 0; j < width; j++) {
			double x = realmin + j * dr;
			double y = imagmin + i * di;
			grid[i][j] = mandelbrot_kernel(x, y, maxiter);
		}
	}
	return grid;
}

inline __m256d mul(__m256d a, __m256d b) {
	return _mm256_mul_pd(a, b);
}

inline __m256d mul(__m256d a, double b) {
	return _mm256_mul_pd(a, _mm256_set1_pd(b));
}

inline __m256d mul(double a, __m256d b) {
	return _mm256_mul_pd(b, _mm256_set1_pd(a));
}

inline __m256d add(__m256d a, __m256d b) {
	return _mm256_add_pd(a, b);
}

inline __m256d sub(__m256d a, __m256d b) {
	return _mm256_sub_pd(a, b);
}

inline __m256d lt(__m256d a, double b) {
	return _mm256_cmp_pd(a, _mm256_set1_pd(b), _CMP_LT_OQ);
}

inline __m256d gt(__m256d a, double b) {
	return _mm256_cmp_pd(a, _mm256_set1_pd(b), _CMP_GT_OQ);
}

std::ostream& operator<<(std::ostream& ost, __m256i a) {
	ll x[4];
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(x), a);
	ost << x[0] << " " << x[1] << " " << x[2] << " " << x[3];
	return ost;
}

__m256i mandelbrot_kernel_avx(
	__m256d real, __m256d imag,
	int maxiter) {
	auto itercnt = _mm256_setzero_si256();
	auto x = real;
	auto y = imag;
	for (std::size_t i = 0; i < maxiter; i++) {
		auto t = add(sub(mul(x,x), mul(y,y)), real);
		y = add(mul(mul(x, y), 2.0), imag);
		x = t;
		auto cmp = lt(add(mul(x, x), mul(y, y)), 4.0);
		auto mask = _mm256_castpd_si256(cmp);
		auto add = _mm256_and_si256(mask, _mm256_set1_epi64x(1));
		itercnt = _mm256_add_epi64(itercnt, add);
		if (_mm256_testz_si256(mask, mask)) {
			break;
		}
	}
	return itercnt;
}

std::vector<std::vector<int>> mandelbrot_avx(
	std::size_t width, std::size_t height,
	double realmax, double realmin,
	double imagmax, double imagmin,
	int maxiter) {
	double dr = (realmax - realmin)/width;
	double di = (imagmax - imagmin)/height;
	auto grid = std::vector<std::vector<int>>(
		height, std::vector<int>(width, 0));
#pragma omp parallel for collapse(1)
	for (std::size_t i = 0; i < height; i++) {
		for (std::size_t j = 0; j < width; j+=4) {
			double y = imagmin + i*di;
			double x[4] = {
				realmin + j*dr,
				realmin + (j+1)*dr,
				realmin + (j+2)*dr,
				realmin + (j+3)*dr};
			__m256d real = _mm256_loadu_pd(x);
			__m256d imag = _mm256_set1_pd(y);
			__m256i itercnt = mandelbrot_kernel_avx(
				real, imag, maxiter);
			ll result[4];
			_mm256_storeu_si256(reinterpret_cast<__m256i*>(result), itercnt);
			for (std::size_t k = 0; k < 4; k++) {
				grid[i][j+k] = result[k];
			}
		}
	}
	return grid;
}
