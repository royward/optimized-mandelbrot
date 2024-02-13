#pragma once

#include <cstdint>

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
	template<class M> void render_avx_sheeprace2(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	template<class M> void render_avx_sheeprace3(iterations_t iterations, uint32_t render_x = 0, uint32_t render_y = 0, uint32_t render_width = 0, uint32_t render_height = 0);
	bool get_next_point(double& pw, double& ph, uint32_t * & dest);
	void reset(iterations_t* pp, uint32_t stride, uint32_t render_width, uint32_t render_height);
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


