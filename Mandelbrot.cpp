#include "Mandelbrot.h"
#include <cstring>

Mandelbrot::Mandelbrot(double cx, double cy, double zoom, uint32_t pwidth, uint32_t pheight) {
	width=pwidth;
	height=pheight;
	p=new iterations_t[width*height+1]();
	xs=cx-0.5/zoom;
	ys=cy+0.5*height/(zoom*width);
	inc=1.0/(zoom*width);
}

void Mandelbrot::reset(iterations_t* pp, uint32_t stride, uint32_t render_width, uint32_t render_height) {
	for(uint32_t i=0;i<render_height;i++) {
		memset(pp+i*stride,0,render_width*sizeof(iterations_t));
	}
}

Mandelbrot::~Mandelbrot() {
	delete[] p;
}
