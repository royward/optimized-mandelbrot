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

template<auto F> uint64_t mandelbrot_render_simple1(iterations_t* p, double cx, double cy, double zoom, uint32_t width, uint32_t height, uint32_t iterations) {
	uint64_t acc = 0;
	double xs = cx - 0.5/zoom;
	double ys = cy + 0.5 * height/(zoom * width);
	double inc = 1.0/(zoom * width);
	for(uint32_t j = 0;j<height;j++) {
		iterations_t* py = p + j * width;
		double y = ys - inc * j;
		for(uint32_t i = 0;i<width;i += 1) {
			py[i] = F(xs + inc * i,y,iterations);
#ifdef POINTS_COUNT
			acc += py[i];
#endif
		}
	}
	return acc;
}

template<auto F> uint64_t mandelbrot_render_simple2(iterations_t* p, double cx, double cy, double zoom, uint32_t width, uint32_t height, uint32_t iterations) {
	uint64_t acc  =  0;
	double xs  =  cx - 0.5 / zoom;
	double ys  =  cy  +  0.5 * height / (zoom * width);
	double inc  =  1.0 / (zoom * width);
	for(uint32_t j  =  0; j < height; j++) {
		iterations_t* py = p  +  j * width;
		double y = ys  -  inc * j;
		for(uint32_t i = 0; i < width; i  +=  2) {
			F(xs  +  inc * i, y, xs  +  inc * (i  +  1),y,iterations, &py[i], &py[i  +  1]);
#ifdef POINTS_COUNT
			acc  +=  py[i]  +  py[i + 1];
#endif
		}
	}
	return acc;
}

template<auto F> uint64_t mandelbrot_render_simple3(iterations_t* p, double cx, double cy, double zoom, uint32_t width, uint32_t height, uint32_t iterations) {
	uint64_t acc = 0;
	double xs = cx - 0.5/zoom;
	double ys = cy + 0.5 * height/(zoom * width);
	double inc = 1.0/(zoom * width);
	for(uint32_t j = 0;j<height;j++) {
		iterations_t* py = p + j * width;
		double y = ys - inc * j;
		for(uint32_t i = 0;i<width;i += 3) {
			F(xs + inc * i,y,xs + inc * (i + 1),y,xs + inc * (i + 2),y,iterations,&py[i],&py[i + 1],&py[i + 2]);
#ifdef POINTS_COUNT
			acc += py[i] + py[i + 1] + py[i + 2];
#endif
		}
	}
	return acc;
}

template<auto F> uint64_t mandelbrot_render_simple4(iterations_t* p, double cx, double cy, double zoom, uint32_t width, uint32_t height, uint32_t iterations) {
	uint64_t acc = 0;
	double xs = cx - 0.5/zoom;
	double ys = cy + 0.5 * height/(zoom * width);
	double inc = 1.0/(zoom * width);
	for(uint32_t j = 0;j<height;j++) {
		iterations_t* py = p + j * width;
		double y = ys - inc * j;
		for(uint32_t i = 0;i<width;i += 4) {
			F(xs + inc * i,y,xs + inc * (i + 1),y,xs + inc * (i + 2),y,xs + inc * (i + 3),y,iterations,&py[i],&py[i + 1],&py[i + 2],&py[i + 3]);
#ifdef POINTS_COUNT
			acc += py[i] + py[i + 1] + py[i + 2] + py[i + 3];
#endif
		}
	}
	return acc;
}

