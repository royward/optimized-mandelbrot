#include "Mandelbrot.h"
#include <string>
#include <iostream>
#include <immintrin.h>
#include "MandelbrotStateEngineSimple.h"
#include "MandelbrotStateEngineEdgeFollow.h"
#include "Mandelbrot_render.h"

uint64_t time_elapsed_ns();

class MandelbrotStateEngineSimple2x {
public:
	MandelbrotStateEngineSimple2x(iterations_t* pp, uint32_t pfullwidth, int32_t pfullheight, uint32_t prender_x, uint32_t prender_y, uint32_t pwidth, uint32_t pheight, double pxs, double pys, double pinc, uint32_t) {
		fullwidth = pfullwidth;
		p = pp + prender_x + fullwidth * prender_y;
		width = pwidth;
		height = pheight;
		dummy_offset = (pfullheight - prender_y - height) * fullwidth - prender_x;
		inc = pinc;
		xs = pxs + prender_x * inc;
		ys = pys - prender_y * inc;
		w = 0;
		h = 0;
		tail = 15;
	};
	bool get_next_point(double& px, double& py, uint32_t&x, uint32_t& y);
	void cleanup() {};
public:
	iterations_t* p;
	uint32_t stored_iterations;
	uint32_t stored_width;
	uint32_t dx[3][4] = {}, dy[3][4] = {};
private:
	int32_t fullwidth;
	uint32_t width;
	uint32_t height;
	int32_t dummy_offset;
	uint32_t w;
	uint32_t h;
	double xs;
	double ys;
	double inc;
	uint32_t tail;
};

//  __attribute__((noinline)) 
inline bool MandelbrotStateEngineSimple2x::get_next_point(double& px, double& py, uint32_t&x, uint32_t& y) {
	if(h == height) {
		if(tail == 0) {
			return false;
		}
		tail--;
		px = py = 0;
		// dummy point
		x = 0;
		y = height;
		return true;
	}
	x = w;
	y = h;
	px = xs + w * inc;
	py = ys - h * inc;
	w++;
	if(w >= width) {
		w = 0;
		h++;
	};
	return true;
}

void Mandelbrot::fastestx(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	MandelbrotStateEngineSimple2x mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
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
		if(test_inline2(mse,pp,width,iterations,s1_dx,s1_dy,cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(test_inline2(mse,pp,width,iterations,s2_dx,s2_dy,cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
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

void Mandelbrot::nt_render_avx_sheeprace2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace3<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_expect(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_expect<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_icmp(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_icmp<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_icmp(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_icmp<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_tinc(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_tinc<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_tinc_addint(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_tinc_addint<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast2<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast_sr(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast_sr<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast_sr2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast_sr2<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast_sr_by2<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast_sr_by3<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace2_u_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace3_u_tinc_addint_fast_sr_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace3_u_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_u_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_tinc_addint_fast_sr_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_tinc_addint_fast_sr_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_tinc_addint_fast_sr_by4_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_sheeprace5_u_tinc_addint_fast_sr_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_sheeprace5_u_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void do_test_fast_math(std::string name, uint32_t count, double cx, double cy) {
	Mandelbrot m(cx,cy,TEST_ZOOM,WIDTH,HEIGHT);
 	uint64_t time0;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_sheeprace1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_sheeprace2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_sheeprace3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_sheeprace4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' ' << "render_sheeprace4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_avx1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_avx2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_avx3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;

// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace3(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_expect(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_expect" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_icmp(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_icmp" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_icmp(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_icmp" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_tinc(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_tinc" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_tinc_addint(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_tinc_addint" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast2(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast_sr(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast_sr" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast_sr2(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast_sr2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by2(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast_sr_by2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by3(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast_sr_by3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast_sr_by4(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace3_u_tinc_addint_fast_sr_by4(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace3_u_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace3_u_tinc_addint_fast_sr_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace3_u_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;

	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace4_u_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;


	// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_macro(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_tinc_addint_fast_sr_by4(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace4_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_tinc_addint_fast_sr_by4_macro(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace4_tinc_addint_fast_sr_by4_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust_macro(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace4_u_tinc_addint_fast_sr_by4_noadjust_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace5_u_tinc_addint_fast_sr_by4(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace5_u_tinc_addint_fast_sr_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace2_u_tinline<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinline" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.fastest(ITERATIONS);
// 	std::cout << name << ' ' << "fastest" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.fastestx(ITERATIONS);
// 	std::cout << name << ' ' << "fastestx1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_multithreaded(ITERATIONS,1,4);
	std::cout << name << ' '  << "thread1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_multithreaded(ITERATIONS,2,4);
	std::cout << name << ' '  << "thread2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_multithreaded(ITERATIONS,3,4);
	std::cout << name << ' '  << "thread3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_multithreaded(ITERATIONS,4,4);
	std::cout << name << ' '  << "thread3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
}

// 	template<class M> void render_avx_sheeprace2_u(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_expect(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_u_tinc(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_u_tinc_addint(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_u_icmp(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
// 	template<class M> void render_avx_sheeprace2_icmp2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
