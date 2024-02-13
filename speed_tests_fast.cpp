#include "Mandelbrot.h"
#include <string>
#include <iostream>
#include "MandelbrotStateEngineSimple.h"
#include "Mandelbrot_render.h"

uint64_t time_elapsed_ns();

void do_test_fast_math(std::string name, uint32_t count, double cx, double cy) {
	Mandelbrot m(cx,cy,TEST_ZOOM,WIDTH,HEIGHT);
 	uint64_t time0;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render1<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render2<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render3<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render4<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace1<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace3<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_sheeprace4<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' ' << "render_sheeprace4" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx1<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx2<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx3<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace1<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace1" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace2" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
	time0=time_elapsed_ns();
	for(uint32_t i=0;i<count;i++)m.render_avx_sheeprace3<MandelbrotStateEngineSimple>(ITERATIONS);
	std::cout << name << ' '  << "render_avx_sheeprace3" << " s=" << (time_elapsed_ns()-time0)*0.000000001 << std::endl;
}
