#include <cstdint>

uint32_t mandelbrot_point(double cx, double cy, uint32_t m) {
	uint32_t count=0;
	double x=0,y=0;
	while(count<m && x*x+y*y<=4.0) {
		double nx=x*x-y*y+cx;
		y=2*x*y+cy;
		x=nx;
		count++;
	}
	return count;
}

