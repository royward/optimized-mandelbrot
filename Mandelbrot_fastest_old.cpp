// BSD 3-Clause License
// 
// Copyright (c) 2024, Roy Ward
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <immintrin.h>
#include "Mandelbrot.h"
#include "MandelbrotStateEngineSimple.h"

void Mandelbrot::fastest_old(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	MandelbrotStateEngineSimple mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x = _mm256_setzero_pd();
	__m256d s1_y = _mm256_setzero_pd();
	__m256d s1_x_squared = _mm256_setzero_pd();
	__m256d s1_y_squared = _mm256_setzero_pd();
	__m256d s1_mag = _mm256_setzero_pd();
	__m256i s1_count = _mm256_set1_epi64x(iterations + 2);
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	double cx_mem[4] __attribute__((__aligned__(32)));
	double cy_mem[4] __attribute__((__aligned__(32)));
	mse.get_next_point(cx_mem[0], cy_mem[0], s1_dx[0], s1_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s1_dx[1], s1_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s1_dx[2], s1_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s1_dx[3], s1_dy[3]);
	__m256d s1_cx = *(__m256d * )cx_mem;
	__m256d s1_cy = *(__m256d * )cy_mem;
	mse.get_next_point(cx_mem[0], cy_mem[0], s2_dx[0], s2_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s2_dx[1], s2_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s2_dx[2], s2_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s2_dx[3], s2_dy[3]);
	__m256d s2_cx = *(__m256d * )cx_mem;
	__m256d s2_cy = *(__m256d * )cy_mem;
	while(true) {
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s1_cx);
			_mm256_store_pd(cy_mem, s1_cy);
			double s1_x_mem[4] __attribute__((__aligned__(32)));
			double s1_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
			double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s1_x_mem, s1_x);
			_mm256_store_pd(s1_y_mem, s1_y);
			_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
			_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
			_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s1_dx[b] + s1_dy[b] * width] = iterations - s1_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s1_dx[b], s1_dy[b])) {
					cleanup(mse);
					return;
				}
				s1_count_mem[b] = iterations + 2;
				s1_x_mem[b] = s1_y_mem[b] = s1_x_squared_mem[b] = s1_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s1_x = _mm256_load_pd(s1_x_mem);
			s1_y = _mm256_load_pd(s1_y_mem);
			s1_count = _mm256_load_si256((__m256i * )s1_count_mem);
			s1_x_squared = _mm256_load_pd(s1_x_squared_mem);
			s1_y_squared = _mm256_load_pd(s1_y_squared_mem);
			s1_cx = _mm256_load_pd(cx_mem);
			s1_cy = _mm256_load_pd(cy_mem);
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s2_cx);
			_mm256_store_pd(cy_mem, s2_cy);
			double s2_x_mem[4] __attribute__((__aligned__(32)));
			double s2_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s2_count_mem[4] __attribute__((__aligned__(32)));
			double s2_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s2_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s2_x_mem, s2_x);
			_mm256_store_pd(s2_y_mem, s2_y);
			_mm256_store_si256((__m256i * )s2_count_mem, s2_count);
			_mm256_store_pd(s2_x_squared_mem, s2_x_squared);
			_mm256_store_pd(s2_y_squared_mem, s2_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s2_dx[b] + s2_dy[b] * width] = iterations - s2_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s2_dx[b], s2_dy[b])) {
					cleanup(mse);
					return;
				}
				s2_count_mem[b] = iterations + 2;
				s2_x_mem[b] = s2_y_mem[b] = s2_x_squared_mem[b] = s2_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s2_x = _mm256_load_pd(s2_x_mem);
			s2_y = _mm256_load_pd(s2_y_mem);
			s2_count = _mm256_load_si256((__m256i * )s2_count_mem);
			s2_x_squared = _mm256_load_pd(s2_x_squared_mem);
			s2_y_squared = _mm256_load_pd(s2_y_squared_mem);
			s2_cx = _mm256_load_pd(cx_mem);
			s2_cy = _mm256_load_pd(cy_mem);
		}
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd(_mm256_add_pd(s2_x, s2_x), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
	}
}
