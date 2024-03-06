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

#pragma once

#include <cstdint>

#ifdef BROADWELL_OR_EARLIER
#define AVX_CMP_GT_4_64(X,Y) _mm256_cmp_pd((X),(Y),_CMP_GT_OS)
#else
#define AVX_CMP_GT_4_64(X,Y) _mm256_cmpgt_epi64((X),(Y))
#endif

typedef uint32_t iterations_t;

const static uint32_t WIDTH=1000;
const static uint32_t HEIGHT=1000;
const static iterations_t ITERATIONS=50000;
const static double TEST_ZOOM=8589934592000;

// Lots of black and detail
const static double TESTA_CX=-0.57245092932760;
const static double TESTA_CY=0.563219321276942;
// All detail
const static double TESTB_CX=-0.57245092932763;
const static double TESTB_CY=0.563219321276842;
// Little black or detail
const static double TESTC_CX=-0.57245092932663;
const static double TESTC_CY=0.563219321276852;
// Black
const static double TESTD_CX=0;
const static double TESTD_CY=0;

class Mandelbrot {
public:
	Mandelbrot(double pcx, double pcy, double pzoom, uint32_t pwidth, uint32_t pheight);
	~Mandelbrot();
	template<class M> void render1(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_sheeprace1(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_sheeprace2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_sheeprace3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_sheeprace4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx1(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace1(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_expect(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u_tinc_addint_fast(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u_tinc_addint_fast2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i2_u_tinc_addint_fast_by2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i2_u_tinc_addint_fast_by3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i2_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i3_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_u_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_u_tinc_addint_fast_by4_noadjust(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace1(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_expect(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u_tinc_addint_fast(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_sheeprace2_u_tinc_addint_fast2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i2_u_tinc_addint_fast_by2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i2_u_tinc_addint_fast_by3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i2_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i3_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_u_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void nt_render_avx_i5_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_i5_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_u_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace2_icmp2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void fastest_old(iterations_t iterations, uint32_t render_x=0, uint32_t render_y=0, uint32_t render_width=0, uint32_t render_height=0);
	void fastest_raw(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void fastest_contour(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	void reset(iterations_t* pp, uint32_t stride, uint32_t render_width, uint32_t render_height);
	void render_multithreaded(uint32_t iterations, uint32_t thread_count, uint32_t divx, uint32_t divy = 0);
	template<class M> void cleanup(M mse) {
		mse.cleanup();
	};
	void save_to_png(const char* name, uint32_t iterations);
	iterations_t* p;
private:
	uint32_t width;
	uint32_t height;
	double xs;
	double ys;
	double inc;
};


