#include <memory>
#include "Mandelbrot.h"
#include <time.h>
#include <string>
#include <iostream>
#include "Mandelbrot_render_simple.h"

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

void do_test(std::string name, uint32_t count, double cx, double cy) {
	uint64_t acc;
	std::unique_ptr<iterations_t[]> p = std::make_unique<iterations_t[]>(HEIGHT*WIDTH+16);
	uint64_t time0;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot_point" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point_fast_math>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot_point_fast_math" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_point_fast_math_decrement>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot_point_fast_math_decrement" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple1<mandelbrot_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple2<mandelbrot2_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot2_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple3<mandelbrot3_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot3_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
	time0=time_elapsed_ns();
	acc=0;
	for(uint32_t i=0;i<count;i++)acc+=mandelbrot_render_simple4<mandelbrot4_sheeprace>(p.get(),cx,cy,TEST_ZOOM,WIDTH,HEIGHT,ITERATIONS);
	std::cout << name << ' ' << "mandelbrot4_sheeprace" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << " c=" << acc << std::endl;
}

void speed_test() {
	if(true) {
		uint32_t COUNT=1;
		do_test("A",COUNT,TESTA_CX,TESTA_CY);
		do_test_fast_math("A",COUNT,TESTA_CX,TESTA_CY);
		do_test("B",COUNT,TESTB_CX,TESTB_CY);
		do_test_fast_math("B",COUNT,TESTB_CX,TESTB_CY);
		do_test("C",COUNT,TESTC_CX,TESTC_CY);
		do_test_fast_math("C",COUNT,TESTC_CX,TESTC_CY);
		do_test("D",COUNT,TESTD_CX,TESTD_CY);
		do_test_fast_math("D",COUNT,TESTD_CX,TESTD_CY);
	}
	std::cout << "Speed tests done" << std::endl;
}

int main() {
	speed_test();
	return 0;
}
