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

#include <immintrin.h>
#include "Mandelbrot.h"

#define RUN_ITERS 32 // number of iterations between iteration overflow checks

template<class M> void Mandelbrot::render1(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s1_mag = 0;
	uint32_t s1_count = iterations;
	uint32_t s1_dx = 0, s1_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations;
		}
		s1_count--;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x_squared = s1_x * s1_x;
		s1_y_squared = s1_y * s1_y;
		s1_mag = s1_x_squared + s1_y_squared;
	}
}

template<class M> void Mandelbrot::render2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s1_mag = 0, s2_mag = 0;
	uint32_t s1_count = iterations;
	uint32_t s2_count = iterations;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations;
		}
		s1_count--;
		s2_count--;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
	}
}

template<class M> void Mandelbrot::render_sheeprace1(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s1_mag = 0;
	uint32_t s1_count = iterations + 2;
	uint32_t s1_dx = 0, s1_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations + 2;
		}
		s1_mag = s1_x_squared + s1_y_squared;
		s1_count--;
		s1_y_squared = s1_y * s1_y;
		s1_x_squared = s1_x * s1_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
	}
}

template<class M> void Mandelbrot::render_sheeprace2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s1_mag = 0, s2_mag = 0;
	uint32_t s1_count = iterations + 2;
	uint32_t s2_count = iterations + 2;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations + 2;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations + 2;
		}
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
		s1_count--;
		s2_count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
	}
}

template<class M> void Mandelbrot::render3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0;
	double s1_mag = 0, s2_mag = 0, s3_mag = 0;
	uint32_t s1_count = iterations;
	uint32_t s2_count = iterations;
	uint32_t s3_count = iterations;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	uint32_t s3_dx = 0, s3_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	double s3_cx = 0, s3_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations;
		}
		if(s3_count == 0 || s3_mag > 4.0) {
			pp[s3_dx + s3_dy * width] = iterations - s3_count;
			if(!mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy)) {
				cleanup(mse);
				return;
			}
			s3_x = s3_y = s3_x_squared = s3_y_squared = s3_mag = 0;
			s3_count = iterations;
		}
		s1_count--;
		s2_count--;
		s3_count--;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s3_y = 2 * s3_y * s3_x + s3_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
		s3_x = s3_x_squared - s3_y_squared + s3_cx;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s3_x_squared = s3_x * s3_x;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s3_y_squared = s3_y * s3_y;
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
		s3_mag = s3_x_squared + s3_y_squared;
	}
}

template<class M> void Mandelbrot::render_sheeprace3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0;
	double s1_mag = 0, s2_mag = 0, s3_mag = 0;
	uint32_t s1_count = iterations + 2;
	uint32_t s2_count = iterations + 2;
	uint32_t s3_count = iterations + 2;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	uint32_t s3_dx = 0, s3_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	double s3_cx = 0, s3_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations + 2;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations + 2;
		}
		if(s3_count == 0 || s3_mag > 4.0) {
			pp[s3_dx + s3_dy * width] = iterations - s3_count;
			if(!mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy)) {
				cleanup(mse);
				return;
			}
			s3_x = s3_y = s3_x_squared = s3_y_squared = s3_mag = 0;
			s3_count = iterations + 2;
		}
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
		s3_mag = s3_x_squared + s3_y_squared;
		s1_count--;
		s2_count--;
		s3_count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s3_y_squared = s3_y * s3_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s3_x_squared = s3_x * s3_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s3_y = 2 * s3_y * s3_x + s3_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
		s3_x = s3_x_squared - s3_y_squared + s3_cx;
	}
}

template<class M> void Mandelbrot::render4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0;
	double s4_x = 0, s4_y = 0, s4_x_squared = 0, s4_y_squared = 0;
	double s1_mag = 0, s2_mag = 0, s3_mag = 0, s4_mag = 0;
	uint32_t s1_count = iterations;
	uint32_t s2_count = iterations;
	uint32_t s3_count = iterations;
	uint32_t s4_count = iterations;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	uint32_t s3_dx = 0, s3_dy = 0;
	uint32_t s4_dx = 0, s4_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	double s3_cx = 0, s3_cy = 0;
	double s4_cx = 0, s4_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy);
	mse.get_next_point(s4_cx, s4_cy, s4_dx, s4_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations;
		}
		if(s3_count == 0 || s3_mag > 4.0) {
			pp[s3_dx + s3_dy * width] = iterations - s3_count;
			if(!mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy)) {
				cleanup(mse);
				return;
			}
			s3_x = s3_y = s3_x_squared = s3_y_squared = s3_mag = 0;
			s3_count = iterations;
		}
		if(s4_count == 0 || s4_mag > 4.0) {
			pp[s4_dx + s4_dy * width] = iterations - s4_count;
			if(!mse.get_next_point(s4_cx, s4_cy, s4_dx, s4_dy)) {
				cleanup(mse);
				return;
			}
			s4_x = s4_y = s4_x_squared = s4_y_squared = s4_mag = 0;
			s4_count = iterations;
		}
		s1_count--;
		s2_count--;
		s3_count--;
		s4_count--;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s3_y = 2 * s3_y * s3_x + s3_cy;
		s4_y = 2 * s4_y * s4_x + s4_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
		s3_x = s3_x_squared - s3_y_squared + s3_cx;
		s4_x = s4_x_squared - s4_y_squared + s4_cx;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s3_x_squared = s3_x * s3_x;
		s4_x_squared = s4_x * s4_x;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s3_y_squared = s3_y * s3_y;
		s4_y_squared = s4_y * s4_y;
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
		s3_mag = s3_x_squared + s3_y_squared;
		s4_mag = s4_x_squared + s4_y_squared;
	}
}

template<class M> void Mandelbrot::render_sheeprace4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0;
	double s4_x = 0, s4_y = 0, s4_x_squared = 0, s4_y_squared = 0;
	double s1_mag = 0, s2_mag = 0, s3_mag = 0, s4_mag = 0;
	uint32_t s1_count = iterations + 2;
	uint32_t s2_count = iterations + 2;
	uint32_t s3_count = iterations + 2;
	uint32_t s4_count = iterations + 2;
	uint32_t s1_dx = 0, s1_dy = 0;
	uint32_t s2_dx = 0, s2_dy = 0;
	uint32_t s3_dx = 0, s3_dy = 0;
	uint32_t s4_dx = 0, s4_dy = 0;
	double s1_cx = 0, s1_cy = 0;
	double s2_cx = 0, s2_cy = 0;
	double s3_cx = 0, s3_cy = 0;
	double s4_cx = 0, s4_cy = 0;
	mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy);
	mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy);
	mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy);
	mse.get_next_point(s4_cx, s4_cy, s4_dx, s4_dy);
	while(true) {
		if(s1_count == 0 || s1_mag > 4.0) {
			pp[s1_dx + s1_dy * width] = iterations - s1_count;
			if(!mse.get_next_point(s1_cx, s1_cy, s1_dx, s1_dy)) {
				cleanup(mse);
				return;
			}
			s1_x = s1_y = s1_x_squared = s1_y_squared = s1_mag = 0;
			s1_count = iterations + 2;
		}
		if(s2_count == 0 || s2_mag > 4.0) {
			pp[s2_dx + s2_dy * width] = iterations - s2_count;
			if(!mse.get_next_point(s2_cx, s2_cy, s2_dx, s2_dy)) {
				cleanup(mse);
				return;
			}
			s2_x = s2_y = s2_x_squared = s2_y_squared = s2_mag = 0;
			s2_count = iterations + 2;
		}
		if(s3_count == 0 || s3_mag > 4.0) {
			pp[s3_dx + s3_dy * width] = iterations - s3_count;
			if(!mse.get_next_point(s3_cx, s3_cy, s3_dx, s3_dy)) {
				cleanup(mse);
				return;
			}
			s3_x = s3_y = s3_x_squared = s3_y_squared = s3_mag = 0;
			s3_count = iterations + 2;
		}
		if(s4_count == 0 || s4_mag > 4.0) {
			pp[s4_dx + s4_dy * width] = iterations - s4_count;
			if(!mse.get_next_point(s4_cx, s4_cy, s4_dx, s4_dy)) {
				cleanup(mse);
				return;
			}
			s4_x = s4_y = s4_x_squared = s4_y_squared = s4_mag = 0;
			s4_count = iterations + 2;
		}
		s1_mag = s1_x_squared + s1_y_squared;
		s2_mag = s2_x_squared + s2_y_squared;
		s3_mag = s3_x_squared + s3_y_squared;
		s4_mag = s4_x_squared + s4_y_squared;
		s1_count--;
		s2_count--;
		s3_count--;
		s4_count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s3_y_squared = s3_y * s3_y;
		s4_y_squared = s4_y * s4_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s3_x_squared = s3_x * s3_x;
		s4_x_squared = s4_x * s4_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s3_y = 2 * s3_y * s3_x + s3_cy;
		s4_y = 2 * s4_y * s4_x + s4_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
		s3_x = s3_x_squared - s3_y_squared + s3_cx;
		s4_x = s4_x_squared - s4_y_squared + s4_cx;
	}
}

template<class M> void Mandelbrot::render_avx1(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x = _mm256_setzero_pd();
	__m256d s1_y = _mm256_setzero_pd();
	__m256d s1_x_squared = _mm256_setzero_pd();
	__m256d s1_y_squared = _mm256_setzero_pd();
	__m256d s1_mag = _mm256_setzero_pd();
	__m256i s1_count = _mm256_set1_epi64x(iterations);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	double cx_mem[4] __attribute__((__aligned__(32)));
	double cy_mem[4] __attribute__((__aligned__(32)));
	mse.get_next_point(cx_mem[0], cy_mem[0], s1_dx[0], s1_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s1_dx[1], s1_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s1_dx[2], s1_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s1_dx[3], s1_dy[3]);
	__m256d s1_cx = *(__m256d * )cx_mem;
	__m256d s1_cy = *(__m256d * )cy_mem;
	while(true) {
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s1_cx);
			_mm256_store_pd(cy_mem, s1_cy);
			double s1_x_mem[4] __attribute__((__aligned__(32)));
			double s1_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
			double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s1_x_mem, s1_x);
			_mm256_store_pd(s1_y_mem, s1_y);
			_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
			_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
			_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s1_dx[b] + s1_dy[b] * width] = iterations - s1_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s1_dx[b], s1_dy[b])) {
					cleanup(mse);
					return;
				}
				s1_count_mem[b] = iterations;
				s1_x_mem[b] = s1_y_mem[b] = s1_x_squared_mem[b] = s1_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s1_x = _mm256_load_pd(s1_x_mem);
			s1_y = _mm256_load_pd(s1_y_mem);
			s1_count = _mm256_load_si256((__m256i * )s1_count_mem);
			s1_x_squared = _mm256_load_pd(s1_x_squared_mem);
			s1_y_squared = _mm256_load_pd(s1_y_squared_mem);
			s1_cx = _mm256_load_pd(cx_mem);
			s1_cy = _mm256_load_pd(cy_mem);
		}
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
	}
}

template<class M> void Mandelbrot::render_avx2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x = _mm256_setzero_pd();
	__m256d s1_y = _mm256_setzero_pd();
	__m256d s1_x_squared = _mm256_setzero_pd();
	__m256d s1_y_squared = _mm256_setzero_pd();
	__m256d s1_mag = _mm256_setzero_pd();
	__m256i s1_count = _mm256_set1_epi64x(iterations);
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
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
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s1_cx);
			_mm256_store_pd(cy_mem, s1_cy);
			double s1_x_mem[4] __attribute__((__aligned__(32)));
			double s1_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
			double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s1_x_mem, s1_x);
			_mm256_store_pd(s1_y_mem, s1_y);
			_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
			_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
			_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s1_dx[b] + s1_dy[b] * width] = iterations - s1_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s1_dx[b], s1_dy[b])) {
					cleanup(mse);
					return;
				}
				s1_count_mem[b] = iterations;
				s1_x_mem[b] = s1_y_mem[b] = s1_x_squared_mem[b] = s1_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s1_x = _mm256_load_pd(s1_x_mem);
			s1_y = _mm256_load_pd(s1_y_mem);
			s1_count = _mm256_load_si256((__m256i * )s1_count_mem);
			s1_x_squared = _mm256_load_pd(s1_x_squared_mem);
			s1_y_squared = _mm256_load_pd(s1_y_squared_mem);
			s1_cx = _mm256_load_pd(cx_mem);
			s1_cy = _mm256_load_pd(cy_mem);
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s2_cx);
			_mm256_store_pd(cy_mem, s2_cy);
			double s2_x_mem[4] __attribute__((__aligned__(32)));
			double s2_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s2_count_mem[4] __attribute__((__aligned__(32)));
			double s2_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s2_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s2_x_mem, s2_x);
			_mm256_store_pd(s2_y_mem, s2_y);
			_mm256_store_si256((__m256i * )s2_count_mem, s2_count);
			_mm256_store_pd(s2_x_squared_mem, s2_x_squared);
			_mm256_store_pd(s2_y_squared_mem, s2_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s2_dx[b] + s2_dy[b] * width] = iterations - s2_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s2_dx[b], s2_dy[b])) {
					cleanup(mse);
					return;
				}
				s2_count_mem[b] = iterations;
				s2_x_mem[b] = s2_y_mem[b] = s2_x_squared_mem[b] = s2_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s2_x = _mm256_load_pd(s2_x_mem);
			s2_y = _mm256_load_pd(s2_y_mem);
			s2_count = _mm256_load_si256((__m256i * )s2_count_mem);
			s2_x_squared = _mm256_load_pd(s2_x_squared_mem);
			s2_y_squared = _mm256_load_pd(s2_y_squared_mem);
			s2_cx = _mm256_load_pd(cx_mem);
			s2_cy = _mm256_load_pd(cy_mem);
		}
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd(_mm256_add_pd(s2_x, s2_x), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
	}
}

template<class M> void Mandelbrot::render_avx3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x = _mm256_setzero_pd();
	__m256d s1_y = _mm256_setzero_pd();
	__m256d s1_x_squared = _mm256_setzero_pd();
	__m256d s1_y_squared = _mm256_setzero_pd();
	__m256d s1_mag = _mm256_setzero_pd();
	__m256i s1_count = _mm256_set1_epi64x(iterations);
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s3_dx[0], s3_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s3_dx[1], s3_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s3_dx[2], s3_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s3_dx[3], s3_dy[3]);
	__m256d s3_cx = *(__m256d * )cx_mem;
	__m256d s3_cy = *(__m256d * )cy_mem;
	while(true) {
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s1_cx);
			_mm256_store_pd(cy_mem, s1_cy);
			double s1_x_mem[4] __attribute__((__aligned__(32)));
			double s1_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
			double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s1_x_mem, s1_x);
			_mm256_store_pd(s1_y_mem, s1_y);
			_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
			_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
			_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s1_dx[b] + s1_dy[b] * width] = iterations - s1_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s1_dx[b], s1_dy[b])) {
					cleanup(mse);
					return;
				}
				s1_count_mem[b] = iterations;
				s1_x_mem[b] = s1_y_mem[b] = s1_x_squared_mem[b] = s1_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s1_x = _mm256_load_pd(s1_x_mem);
			s1_y = _mm256_load_pd(s1_y_mem);
			s1_count = _mm256_load_si256((__m256i * )s1_count_mem);
			s1_x_squared = _mm256_load_pd(s1_x_squared_mem);
			s1_y_squared = _mm256_load_pd(s1_y_squared_mem);
			s1_cx = _mm256_load_pd(cx_mem);
			s1_cy = _mm256_load_pd(cy_mem);
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s2_cx);
			_mm256_store_pd(cy_mem, s2_cy);
			double s2_x_mem[4] __attribute__((__aligned__(32)));
			double s2_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s2_count_mem[4] __attribute__((__aligned__(32)));
			double s2_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s2_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s2_x_mem, s2_x);
			_mm256_store_pd(s2_y_mem, s2_y);
			_mm256_store_si256((__m256i * )s2_count_mem, s2_count);
			_mm256_store_pd(s2_x_squared_mem, s2_x_squared);
			_mm256_store_pd(s2_y_squared_mem, s2_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s2_dx[b] + s2_dy[b] * width] = iterations - s2_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s2_dx[b], s2_dy[b])) {
					cleanup(mse);
					return;
				}
				s2_count_mem[b] = iterations;
				s2_x_mem[b] = s2_y_mem[b] = s2_x_squared_mem[b] = s2_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s2_x = _mm256_load_pd(s2_x_mem);
			s2_y = _mm256_load_pd(s2_y_mem);
			s2_count = _mm256_load_si256((__m256i * )s2_count_mem);
			s2_x_squared = _mm256_load_pd(s2_x_squared_mem);
			s2_y_squared = _mm256_load_pd(s2_y_squared_mem);
			s2_cx = _mm256_load_pd(cx_mem);
			s2_cy = _mm256_load_pd(cy_mem);
		}
		cmp2 = _mm256_or_pd((__m256d)s3_count, _mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s3_cx);
			_mm256_store_pd(cy_mem, s3_cy);
			double s3_x_mem[4] __attribute__((__aligned__(32)));
			double s3_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s3_count_mem[4] __attribute__((__aligned__(32)));
			double s3_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s3_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s3_x_mem, s3_x);
			_mm256_store_pd(s3_y_mem, s3_y);
			_mm256_store_si256((__m256i * )s3_count_mem, s3_count);
			_mm256_store_pd(s3_x_squared_mem, s3_x_squared);
			_mm256_store_pd(s3_y_squared_mem, s3_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s3_dx[b] + s3_dy[b] * width] = iterations - s3_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s3_dx[b], s3_dy[b])) {
					cleanup(mse);
					return;
				}
				s3_count_mem[b] = iterations;
				s3_x_mem[b] = s3_y_mem[b] = s3_x_squared_mem[b] = s3_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s3_x = _mm256_load_pd(s3_x_mem);
			s3_y = _mm256_load_pd(s3_y_mem);
			s3_count = _mm256_load_si256((__m256i * )s3_count_mem);
			s3_x_squared = _mm256_load_pd(s3_x_squared_mem);
			s3_y_squared = _mm256_load_pd(s3_y_squared_mem);
			s3_cx = _mm256_load_pd(cx_mem);
			s3_cy = _mm256_load_pd(cy_mem);
		}
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd(_mm256_add_pd(s2_x, s2_x), s2_y, s2_cy);
		s3_y = _mm256_fmadd_pd(_mm256_add_pd(s3_x, s3_x), s3_y, s3_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
		s3_x = _mm256_sub_pd(s3_x_squared, _mm256_sub_pd(s3_y_squared, s3_cx));
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
	}
}

template<class M> void Mandelbrot::render_avx_sheeprace1(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x = _mm256_setzero_pd();
	__m256d s1_y = _mm256_setzero_pd();
	__m256d s1_x_squared = _mm256_setzero_pd();
	__m256d s1_y_squared = _mm256_setzero_pd();
	__m256d s1_mag = _mm256_setzero_pd();
	__m256i s1_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	double cx_mem[4] __attribute__((__aligned__(32)));
	double cy_mem[4] __attribute__((__aligned__(32)));
	mse.get_next_point(cx_mem[0], cy_mem[0], s1_dx[0], s1_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s1_dx[1], s1_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s1_dx[2], s1_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s1_dx[3], s1_dy[3]);
	__m256d s1_cx = *(__m256d * )cx_mem;
	__m256d s1_cy = *(__m256d * )cy_mem;
	while(true) {
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) {
			uint32_t mask = _mm256_movemask_pd(cmp2);
			_mm256_store_pd(cx_mem, s1_cx);
			_mm256_store_pd(cy_mem, s1_cy);
			double s1_x_mem[4] __attribute__((__aligned__(32)));
			double s1_y_mem[4] __attribute__((__aligned__(32)));
			uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
			double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
			double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
			_mm256_store_pd(s1_x_mem, s1_x);
			_mm256_store_pd(s1_y_mem, s1_y);
			_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
			_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
			_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
			while(mask != 0) {
				uint32_t b = __builtin_ctz(mask);
				pp[s1_dx[b] + s1_dy[b] * width] = iterations - s1_count_mem[b];
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], s1_dx[b], s1_dy[b])) {
					cleanup(mse);
					return;
				}
				s1_count_mem[b] = iterations + 2;
				s1_x_mem[b] = s1_y_mem[b] = s1_x_squared_mem[b] = s1_y_squared_mem[b] = 0;
				mask = mask&~(1U<<b);
			}
			s1_x = _mm256_load_pd(s1_x_mem);
			s1_y = _mm256_load_pd(s1_y_mem);
			s1_count = _mm256_load_si256((__m256i * )s1_count_mem);
			s1_x_squared = _mm256_load_pd(s1_x_squared_mem);
			s1_y_squared = _mm256_load_pd(s1_y_squared_mem);
			s1_cx = _mm256_load_pd(cx_mem);
			s1_cy = _mm256_load_pd(cy_mem);
		}
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
	}
}

template<class M> void Mandelbrot::render_avx_sheeprace2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	iterations--;
	__m256d s1_x        = _mm256_setzero_pd();
	__m256d s1_y        = _mm256_setzero_pd();
	__m256d s1_mag      = _mm256_setzero_pd();
	__m256i s1_count    = _mm256_set1_epi64x(iterations + 2);
	__m256d s2_x        = _mm256_setzero_pd();
	__m256d s2_y        = _mm256_setzero_pd();
	__m256d s2_mag      = _mm256_setzero_pd();
	__m256i s2_count    = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64   = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i t1, t2;
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	double   cx_mem[4]    __attribute__((__aligned__(32)));
	double   cy_mem[4]    __attribute__((__aligned__(32)));
	double   x_mem[4]     __attribute__((__aligned__(32)));
	double   y_mem[4]     __attribute__((__aligned__(32)));
	uint64_t count_mem[4] __attribute__((__aligned__(32)));

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
#		define TEST(x,y,cx,cy,dx,dy,count) \
		if(!_mm256_testz_pd(t1, t1)) [[unlikely]] { \
			uint32_t mask = _mm256_movemask_pd(t1); \
			_mm256_store_pd(cx_mem, cx); \
			_mm256_store_pd(cy_mem, cy); \
			_mm256_store_pd(x_mem, x); \
			_mm256_store_pd(y_mem, y); \
			_mm256_store_si256((__m256i * )count_mem, count); \
			while(mask != 0) { \
				uint32_t b = __builtin_ctz(mask); \
				pp[dx[b] + dy[b] * width] = iterations - count_mem[b]; \
				if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) { \
					cleanup(mse); \
					return; \
				} \
				count_mem[b] = iterations + 2; \
				x_mem[b] = y_mem[b] = 0; \
				mask = mask & ~(1U<<b); \
			} \
			x = _mm256_load_pd(x_mem); \
			y = _mm256_load_pd(y_mem); \
			count = _mm256_load_si256((__m256i * )count_mem); \
			cx = _mm256_load_pd(cx_mem); \
			cy = _mm256_load_pd(cy_mem); \
		}

		t1 = _mm256_or_pd((__m256d)s1_count, _mm256_cmpgt_epi64(s1_mag, four_double));
		TEST(s1_x, s1_y, s1_cx, s1_cy, s1_dx, s1_dy, s1_count);
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s1_mag   = _mm256_fmadd_pd(s1_x, s1_x, s1_y);
		t1       = _mm256_add_pd(s1_x, s1_x);
		t1       = _mm256_fmadd_pd(t1, s1_y, s1_cy);
		s1_y     = _mm256_mul_pd(s1_y, s1_y);
		t2       = _mm256_fmsub_pd(s1_x, s1_x, s1_y);
		s1_x     = _mm256_add_pd(t2, s1_cx);
		s1_y     = t1;

		t1 = _mm256_or_pd((__m256d)s2_count, _mm256_cmpgt_epi64(s2_mag, four_double));
		TEST(s2_x, s2_y, s2_cx, s2_cy, s2_dx, s2_dy, s2_count);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s2_mag   = _mm256_fmadd_pd(s2_x, s2_x, s2_y);
		t1       = _mm256_add_pd(s2_x, s2_x);
		t1       = _mm256_fmadd_pd(t1, s2_y, s2_cy);
		s2_y     = _mm256_mul_pd(s2_y, s2_y);
		t2       = _mm256_fmsub_pd(s2_x, s2_x, s2_y);
		s2_x     = _mm256_add_pd(t2, s2_cx);
		s2_y     = t1;
	}
}

inline void update_point(__m256d &x, __m256d &y, __m256d &cx, __m256d &cy, __m256i &t1) {
	__m256i t2;
	t1    = _mm256_add_pd(x, x);
	t1    = _mm256_fmadd_pd(t1, y, cy);
	y     = _mm256_mul_pd(y, y);
	t2    = _mm256_fmsub_pd(x, x, y);
	x     = _mm256_add_pd(t2, cx);
	y     = t1;
	t1    = _mm256_fmadd_pd(x, x, y);
	t1    = _mm256_cmpgt_epi64(t1, _mm256_set1_pd(4.0));
}

template<class M> inline bool update_avx_stream(M& mse, __m256d &x, __m256d &y, __m256d &cx, __m256d &cy, __m256i mask_vec, int i, uint32_t *dx, uint32_t *dy, uint64_t *count, iterations_t iterations, int width) {
	double cx_mem[4] __attribute__((__aligned__(32)));
	double cy_mem[4] __attribute__((__aligned__(32)));
	double x_mem[4]  __attribute__((__aligned__(32)));
	double y_mem[4]  __attribute__((__aligned__(32)));

	uint32_t mask = _mm256_movemask_pd(mask_vec);
	_mm256_store_pd(cx_mem, cx);
	_mm256_store_pd(cy_mem, cy);
	_mm256_store_pd(x_mem, x);
	_mm256_store_pd(y_mem, y);

	while(mask != 0) {
		uint32_t b = __builtin_ctz(mask);

		mse.p[dx[b] + dy[b] * width] = (i + count[b]>iterations) ? iterations : i + count[b];
		if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
			mse.cleanup();
			return false;
		}
		count[b] = -i;
		x_mem[b] = y_mem[b] = 0;
		mask = mask & ~(1U<<b);
	}
	x     = _mm256_load_pd(x_mem);
	y     = _mm256_load_pd(y_mem);
	cx    = _mm256_load_pd(cx_mem);
	cy    = _mm256_load_pd(cy_mem);

	return true;
}

template<class M> void Mandelbrot::render_avx_sheeprace3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
	if(render_width == 0) {
		render_width = width - render_x;
	}
	if(render_height == 0) {
		render_height = height - render_y;
	}
	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
	iterations_t* pp = mse.p;
	reset(pp, width, render_width, render_height);
	__m256d s1_x        = _mm256_setzero_pd();
	__m256d s1_y        = _mm256_setzero_pd();
	__m256d s2_x        = _mm256_setzero_pd();
	__m256d s2_y        = _mm256_setzero_pd();
	__m256d s3_x        = _mm256_setzero_pd();
	__m256d s3_y        = _mm256_setzero_pd();
	__m256d run_iters   = _mm256_set1_epi64x(RUN_ITERS);
	__m256d max_iters   = _mm256_set1_epi64x(iterations);
	__m256i t1;
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	double   cx_mem[4]    __attribute__((__aligned__(32)));
	double   cy_mem[4]    __attribute__((__aligned__(32)));
	uint64_t s1_count[4] __attribute__((__aligned__(32))) = {0};
	uint64_t s2_count[4] __attribute__((__aligned__(32))) = {0};
	uint64_t s3_count[4] __attribute__((__aligned__(32))) = {0};

	mse.get_next_point(cx_mem[0], cy_mem[0], s1_dx[0], s1_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s1_dx[1], s1_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s1_dx[2], s1_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s1_dx[3], s1_dy[3]);
	__m256d s1_cx = _mm256_load_pd(cx_mem);
	__m256d s1_cy = _mm256_load_pd(cy_mem);
	mse.get_next_point(cx_mem[0], cy_mem[0], s2_dx[0], s2_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s2_dx[1], s2_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s2_dx[2], s2_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s2_dx[3], s2_dy[3]);
	__m256d s2_cx = _mm256_load_pd(cx_mem);
	__m256d s2_cy = _mm256_load_pd(cy_mem);
	mse.get_next_point(cx_mem[0], cy_mem[0], s3_dx[0], s3_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s3_dx[1], s3_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s3_dx[2], s3_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s3_dx[3], s3_dy[3]);
	__m256d s3_cx = _mm256_load_pd(cx_mem);
	__m256d s3_cy = _mm256_load_pd(cy_mem);

	while(true) {
		for (int i=0; i<RUN_ITERS; i++) {
			update_point(s1_x, s1_y, s1_cx, s1_cy, t1);
			if(!_mm256_testz_pd(t1, t1)) [[unlikely]] {
				if (!update_avx_stream<M>(mse, s1_x, s1_y, s1_cx, s1_cy, t1, i+1, s1_dx, s1_dy, s1_count, iterations, width)) {
					return;
				}
			}

			update_point(s2_x, s2_y, s2_cx, s2_cy, t1);
			if(!_mm256_testz_pd(t1, t1)) [[unlikely]] {
				if (!update_avx_stream<M>(mse, s2_x, s2_y, s2_cx, s2_cy, t1, i+1, s2_dx, s2_dy, s2_count, iterations, width)) {
					return;
				}
			}

			update_point(s3_x, s3_y, s3_cx, s3_cy, t1);
			if(!_mm256_testz_pd(t1, t1)) [[unlikely]] {
				if (!update_avx_stream<M>(mse, s3_x, s3_y, s3_cx, s3_cy, t1, i+1, s3_dx, s3_dy, s3_count, iterations, width)) {
					return;
				}
			}
		}

		t1 = _mm256_add_epi64(_mm256_load_si256((__m256i*)s1_count), run_iters);
		_mm256_store_si256((__m256i*)s1_count, t1);
		t1 = _mm256_cmpgt_epi64(t1, max_iters);
		if (!_mm256_testz_pd(t1, t1)) [[unlikely]] {
			if (!update_avx_stream<M>(mse, s1_x, s1_y, s1_cx, s1_cy, t1, 0, s1_dx, s1_dy, s1_count, iterations, width)) return;
		}

		t1 = _mm256_add_epi64(_mm256_load_si256((__m256i*)s2_count), run_iters);
		_mm256_store_si256((__m256i*)s2_count, t1);
		t1 = _mm256_cmpgt_epi64(t1, max_iters);
		if (!_mm256_testz_pd(t1, t1)) [[unlikely]] {
			if (!update_avx_stream<M>(mse, s2_x, s2_y, s2_cx, s2_cy, t1, 0, s2_dx, s2_dy, s2_count, iterations, width)) return;
		}

		t1 = _mm256_add_epi64(_mm256_load_si256((__m256i*)s3_count), run_iters);
		_mm256_store_si256((__m256i*)s3_count, t1);
		t1 = _mm256_cmpgt_epi64(t1, max_iters);
		if (!_mm256_testz_pd(t1, t1)) [[unlikely]] {
			if (!update_avx_stream<M>(mse, s3_x, s3_y, s3_cx, s3_cy, t1, 0, s3_dx, s3_dy, s3_count, iterations, width)) return;
		}
	}
}