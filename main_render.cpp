#include "Mandelbrot.h"
#include "Mandelbrot_render.h"
#include "MandelbrotStateEngineSimple.h"

int main() {
	Mandelbrot m1(TESTA_CX,TESTA_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m1.render_avx_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	m1.save_to_png("test_black_detail.png",ITERATIONS);
	Mandelbrot m2(TESTB_CX,TESTB_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m2.render_avx_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	m2.save_to_png("test_noblack_detail.png",ITERATIONS);
	Mandelbrot m3(TESTC_CX,TESTC_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m3.render_avx_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	m3.save_to_png("test_noblack_nodetail.png",ITERATIONS);
	Mandelbrot m4(TESTD_CX,TESTD_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m4.render_avx_sheeprace2<MandelbrotStateEngineSimple>(ITERATIONS);
	m4.save_to_png("blacka.png",ITERATIONS);
	Mandelbrot m5(0,0,0.25,WIDTH,HEIGHT);
	m5.render_avx_sheeprace3<MandelbrotStateEngineSimple>(ITERATIONS);
	m5.save_to_png("mandelbrot.png",ITERATIONS);
	return 0;

}
