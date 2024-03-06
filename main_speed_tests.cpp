#include <memory>
#include "Mandelbrot.h"
#include <time.h>
#include <string>
#include <iostream>
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

#include "Mandelbrot_render_simple.h"
#include "Mandelbrot_render.h"
#include "MandelbrotStateEngineEdgeFollow.h"

uint64_t time_elapsed_ns() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	uint64_t ret=ts.tv_sec;
	return ret*1000000000ULL+ts.tv_nsec;
}

uint32_t mandelbrot_point(double cx, double cy, uint32_t m);
uint32_t mandelbrot_point_fast_math(double cx, double cy, uint32_t m);
uint32_t mandelbrot_point_fast_math_decrement(double cx, double cy, uint32_t m);
uint32_t mandelbrot_sheeprace(double cx, double cy, uint32_t m);
void mandelbrot2_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r);
void mandelbrot3_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, double s3_cx, double s3_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r, iterations_t* s3_r);
void mandelbrot4_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, double s3_cx, double s3_cy, double s4_cx, double s4_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r, iterations_t* s3_r, iterations_t* s4_r);
void do_test_fast_math(std::string name, uint32_t count, double cx, double cy);

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

void Mandelbrot::nt_render_avx_i2_u_tinc_addint_fast_by2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i2_u_tinc_addint_fast_by2<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i2_u_tinc_addint_fast_by3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i2_u_tinc_addint_fast_by3<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i2_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i2_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i3_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i3_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_u_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_u_tinc_addint_fast_by4_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_tinc_addint_fast_by4_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_u_tinc_addint_fast_by4_noadjust<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void Mandelbrot::nt_render_avx_i5_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	render_avx_i5_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(iterations,render_x,render_y,render_width,render_height);
}

void do_test(std::string name, uint32_t count, double cx, double cy) {
	uint64_t acc;
	std::unique_ptr<iterations_t[]> p = std::make_unique<iterations_t[]>(HEIGHT*WIDTH+16);
	uint64_t time0;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot_point" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point_fast_math>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot_point_fast_math" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point_fast_math_decrement>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot_point_fast_math_decrement" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple2<mandelbrot2_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot2_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple3<mandelbrot3_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot3_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
// 	time0=time_elapsed_ns();
// 	acc=0;
// 	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple4<mandelbrot4_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
// 	std::cout << name << ' ' << "mandelbrot4_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;

 	Mandelbrot m(cx,cy,TEST_ZOOM,WIDTH,HEIGHT);
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' '  << "render_avx1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx2<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' '  << "render_avx2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx3<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' '  << "render_avx3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace1<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;

	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace3(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_expect(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_expect" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_icmp(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u_icmp" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_icmp(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_icmp" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_tinc(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_tinc" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_tinc_addint(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_tinc_addint" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_sheeprace2_u_tinc_addint_fast2(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2_u_tinc_addint_fast2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
// 	time0=time_elapsed_ns();
// 	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i2_u_tinc_addint_fast_by2(ITERATIONS);
// 	std::cout << name << ' '  << "render_avx_i2_u_tinc_addint_fast_by2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i2_u_tinc_addint_fast_by3(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i2_u_tinc_addint_fast_by3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i2_u_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i2_u_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i3_u_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i3_u_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i3_u_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i3_u_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;

	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_u_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_u_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i5_u_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i5_u_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;


	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_u_tinc_addint_fast_by4_macro(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_u_tinc_addint_fast_by4_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_tinc_addint_fast_by4(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_tinc_addint_fast_by4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_tinc_addint_fast_by4_macro(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_tinc_addint_fast_by4_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_u_tinc_addint_fast_by4_noadjust" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.nt_render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro(ITERATIONS);
	std::cout << name << ' '  << "render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;

	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.fastest(ITERATIONS);
	std::cout << name << ' ' << "fastest" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
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

void speed_test() {
	if(true) {
		uint32_t COUNT=1;
 		do_test("A",COUNT,TESTA_CX,TESTA_CY);
 		do_test("B",COUNT,TESTB_CX,TESTB_CY);
 		do_test("C",COUNT,TESTC_CX,TESTC_CY);
		do_test("D",COUNT,TESTD_CX,TESTD_CY);
	}
	std::cout << "Speed tests done" << std::endl;
}

int main() {
	speed_test();
	return 0;
}
