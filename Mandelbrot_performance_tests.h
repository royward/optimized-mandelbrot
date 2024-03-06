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

// ***NOTE*** These are various mixed timing tests. Some of them are broken. Use at your own risk.

#pragma once

#include <immintrin.h>
#include "Mandelbrot.h"
#include <iostream>

using namespace std;

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
				s2_count_mem[b] = iterations + 2;
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

template<class M> void Mandelbrot::render_avx_sheeprace2_icmp(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmpgt_epi64(s1_mag, four_double));
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
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmpgt_epi64(s2_mag, four_double));
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
				s2_count_mem[b] = iterations + 2;
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

template<class M> void Mandelbrot::render_avx_sheeprace2_u_icmp(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmpgt_epi64(s1_mag, four_double));
		if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
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
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmpgt_epi64(s2_mag, four_double));
		if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
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
				s2_count_mem[b] = iterations + 2;
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

template<class M> void Mandelbrot::render_avx_sheeprace2_u(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
		if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
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
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
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
				s2_count_mem[b] = iterations + 2;
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

template<class M> inline bool test_inline2(M& mse, iterations_t* pp, uint32_t width, iterations_t iterations, uint32_t* dx, uint32_t* dy,__m256d& cmp2,__m256d& cx,__m256d& cy,__m256d& x,__m256d& y,__m256d& x_squared,__m256d& y_squared,__m256i& count) {
	if(!_mm256_testz_pd(cmp2, cmp2)) {
		uint32_t mask = _mm256_movemask_pd(cmp2);
		double cx_mem[4] __attribute__((__aligned__(32)));
		double cy_mem[4] __attribute__((__aligned__(32)));
		double x_mem[4] __attribute__((__aligned__(32)));
		double y_mem[4] __attribute__((__aligned__(32)));
		uint64_t count_mem[4] __attribute__((__aligned__(32)));
		double x_squared_mem[4] __attribute__((__aligned__(32)));
		double y_squared_mem[4] __attribute__((__aligned__(32)));
		_mm256_store_pd(cx_mem, cx);
		_mm256_store_pd(cy_mem, cy);
		_mm256_store_pd(x_mem, x);
		_mm256_store_pd(y_mem, y);
		_mm256_store_si256((__m256i * )count_mem, count);
		_mm256_store_pd(x_squared_mem, x_squared);
		_mm256_store_pd(y_squared_mem, y_squared);
		while(mask != 0) {
			uint32_t b = __builtin_ctz(mask);
			pp[dx[b] + dy[b] * width] = iterations - count_mem[b];
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
				return true;
			}
			count_mem[b] = iterations + 2;
			x_mem[b] = y_mem[b] = x_squared_mem[b] = y_squared_mem[b] = 0;
			mask = mask&~(1U<<b);
		}
		x = _mm256_load_pd(x_mem);
		y = _mm256_load_pd(y_mem);
		count = _mm256_load_si256((__m256i * )count_mem);
		x_squared = _mm256_load_pd(x_squared_mem);
		y_squared = _mm256_load_pd(y_squared_mem);
		cx = _mm256_load_pd(cx_mem);
		cy = _mm256_load_pd(cy_mem);
	}
	return false;
}

template<class M> inline bool test_inline2u(M& mse, iterations_t* pp, uint32_t width, iterations_t iterations, uint32_t* dx, uint32_t* dy,__m256d& cmp2,__m256d& cx,__m256d& cy,__m256d& x,__m256d& y,__m256d& x_squared,__m256d& y_squared,__m256i& count) {
	if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
		uint32_t mask = _mm256_movemask_pd(cmp2);
		double cx_mem[4] __attribute__((__aligned__(32)));
		double cy_mem[4] __attribute__((__aligned__(32)));
		double x_mem[4] __attribute__((__aligned__(32)));
		double y_mem[4] __attribute__((__aligned__(32)));
		uint64_t count_mem[4] __attribute__((__aligned__(32)));
		double x_squared_mem[4] __attribute__((__aligned__(32)));
		double y_squared_mem[4] __attribute__((__aligned__(32)));
		_mm256_store_pd(cx_mem, cx);
		_mm256_store_pd(cy_mem, cy);
		_mm256_store_pd(x_mem, x);
		_mm256_store_pd(y_mem, y);
		_mm256_store_si256((__m256i * )count_mem, count);
		_mm256_store_pd(x_squared_mem, x_squared);
		_mm256_store_pd(y_squared_mem, y_squared);
		while(mask != 0) {
			uint32_t b = __builtin_ctz(mask);
			pp[dx[b] + dy[b] * width] = iterations - count_mem[b];
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
				return true;
			}
			count_mem[b] = iterations + 2;
			x_mem[b] = y_mem[b] = x_squared_mem[b] = y_squared_mem[b] = 0;
			mask = mask&~(1U<<b);
		}
		x = _mm256_load_pd(x_mem);
		y = _mm256_load_pd(y_mem);
		count = _mm256_load_si256((__m256i * )count_mem);
		x_squared = _mm256_load_pd(x_squared_mem);
		y_squared = _mm256_load_pd(y_squared_mem);
		cx = _mm256_load_pd(cx_mem);
		cy = _mm256_load_pd(cy_mem);
	}
	return false;
}

template<class M> void Mandelbrot::render_avx_sheeprace2_tinc(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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

template<class M> void Mandelbrot::render_avx_sheeprace2_tinc_addint(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
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
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
	}
}

template<class M> void Mandelbrot::render_avx_sheeprace2_u_tinc(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
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

template<class M> void Mandelbrot::render_avx_sheeprace2_u_tinc_addint(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
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
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
	}
}

template<class M> void Mandelbrot::render_avx_sheeprace2_u_tinc_addint_fast(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
		}
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		// make a temp copy of y
		__m256d s1_temp_y = s1_y;
		__m256d s2_temp_y = s2_y;
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_temp_y, s1_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_temp_y, s2_temp_y, s2_cx));
	}
}

template<class M> void Mandelbrot::render_avx_sheeprace2_u_tinc_addint_fast2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(1);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
		}
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		// make a temp copy of y
		__m256d s1_temp_y = s1_y;
		__m256d s2_temp_y = s2_y;
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_temp_y, s1_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_temp_y, s2_temp_y, s2_cx));
	}
}

template<class M> void Mandelbrot::render_avx_i2_u_tinc_addint_fast_by2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(2);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
		}
		// make a temp copy of y
		__m256d s1_temp_y = s1_y;
		__m256d s2_temp_y = s2_y;
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_temp_y, s1_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_temp_y, s2_temp_y, s2_cx));

		// make a temp copy of y
		__m256d s1b_temp_y = s1_y;
		__m256d s2b_temp_y = s2_y;
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));

	}
}

template<class M> void Mandelbrot::render_avx_i2_u_tinc_addint_fast_by3(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(3);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
		}
		// make a temp copy of y
		__m256d s1_temp_y = s1_y;
		__m256d s2_temp_y = s2_y;
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_temp_y, s1_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_temp_y, s2_temp_y, s2_cx));

		__m256d s1b_temp_y = s1_y;
		__m256d s2b_temp_y = s2_y;
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));

		__m256d s1c_temp_y = s1_y;
		__m256d s2c_temp_y = s2_y;
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1c_temp_y, s1c_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2c_temp_y, s2c_temp_y, s2_cx));

	}
}

template<class M> void Mandelbrot::render_avx_i2_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
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
		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
			cleanup(mse);
			return;
		}
		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
			cleanup(mse);
			return;
		}
		// make a temp copy of y
		__m256d s1_temp_y = s1_y;
		__m256d s2_temp_y = s2_y;
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_temp_y, s1_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_temp_y, s2_temp_y, s2_cx));

		__m256d s1b_temp_y = s1_y;
		__m256d s2b_temp_y = s2_y;
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));

		__m256d s1c_temp_y = s1_y;
		__m256d s2c_temp_y = s2_y;
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1c_temp_y, s1c_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2c_temp_y, s2c_temp_y, s2_cx));

		__m256d s1d_temp_y = s1_y;
		__m256d s2d_temp_y = s2_y;
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1d_temp_y, s1d_temp_y, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2d_temp_y, s2d_temp_y, s2_cx));

	}
}

#define test_diverge_def(dx,dy,cmp2,cx,cy,x,y,count,x_safe0,y_safe0,count_safe0,x_safe1,y_safe1,count_safe1) \
	if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] { \
		uint32_t mask = _mm256_movemask_pd(cmp2); \
		double cx_mem[4] __attribute__((__aligned__(32))); \
		double cy_mem[4] __attribute__((__aligned__(32))); \
		double x_mem[4] __attribute__((__aligned__(32))); \
		double y_mem[4] __attribute__((__aligned__(32))); \
		uint64_t count_mem[4] __attribute__((__aligned__(32))); \
		_mm256_store_pd(x_mem, x); \
		_mm256_store_pd(y_mem, y); \
		_mm256_store_si256((__m256i * )count_mem, count); \
		_mm256_store_pd(cx_mem, cx); \
		_mm256_store_pd(cy_mem, cy); \
		while(mask != 0) { \
			uint32_t b = __builtin_ctz(mask); \
			int64_t si=count_safe1[b]; \
			double sx=x_safe1[b]; \
			double sy=y_safe1[b]; \
			double scx=cx_mem[b]; \
			double scy=cy_mem[b]; \
			while(si >= 0 && sx * sx + sy * sy <= 4.0) { \
				double nx = sx * sx - sy * sy + scx; \
				sy = 2.0 * sx * sy + scy; \
				sx = nx; \
				si--; \
			} \
			pp[dx[b] + dy[b] * width] = iterations - si; \
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) { \
				cleanup(mse); \
				return; \
			} \
			count_mem[b] = iterations; \
			x_mem[b] = y_mem[b] = 0; \
			mask = mask&~(1U<<b); \
		} \
		x = _mm256_load_pd(x_mem); \
		y = _mm256_load_pd(y_mem); \
		count = _mm256_load_si256((__m256i * )count_mem); \
		cx = _mm256_load_pd(cx_mem); \
		cy = _mm256_load_pd(cy_mem); \
	} \
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0)); \
	_mm256_store_pd(x_safe0, x); \
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0)); \
	_mm256_store_pd(y_safe0, y); \
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0)); \
	_mm256_store_si256((__m256i * )count_safe0, count);

#define test_diverge_def_nou(dx,dy,cmp2,cx,cy,x,y,count,x_safe0,y_safe0,count_safe0,x_safe1,y_safe1,count_safe1) \
	if(!_mm256_testz_pd(cmp2, cmp2)) { \
		uint32_t mask = _mm256_movemask_pd(cmp2); \
		double cx_mem[4] __attribute__((__aligned__(32))); \
		double cy_mem[4] __attribute__((__aligned__(32))); \
		double x_mem[4] __attribute__((__aligned__(32))); \
		double y_mem[4] __attribute__((__aligned__(32))); \
		uint64_t count_mem[4] __attribute__((__aligned__(32))); \
		_mm256_store_pd(x_mem, x); \
		_mm256_store_pd(y_mem, y); \
		_mm256_store_si256((__m256i * )count_mem, count); \
		_mm256_store_pd(cx_mem, cx); \
		_mm256_store_pd(cy_mem, cy); \
		while(mask != 0) { \
			uint32_t b = __builtin_ctz(mask); \
			int64_t si=count_safe1[b]; \
			double sx=x_safe1[b]; \
			double sy=y_safe1[b]; \
			double scx=cx_mem[b]; \
			double scy=cy_mem[b]; \
			while(si >= 0 && sx * sx + sy * sy <= 4.0) { \
				double nx = sx * sx - sy * sy + scx; \
				sy = 2.0 * sx * sy + scy; \
				sx = nx; \
				si--; \
			} \
			pp[dx[b] + dy[b] * width] = iterations - si; \
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) { \
				cleanup(mse); \
				return; \
			} \
			count_mem[b] = iterations; \
			x_mem[b] = y_mem[b] = 0; \
			mask = mask&~(1U<<b); \
		} \
		x = _mm256_load_pd(x_mem); \
		y = _mm256_load_pd(y_mem); \
		count = _mm256_load_si256((__m256i * )count_mem); \
		cx = _mm256_load_pd(cx_mem); \
		cy = _mm256_load_pd(cy_mem); \
	} \
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0)); \
	_mm256_store_pd(x_safe0, x); \
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0)); \
	_mm256_store_pd(y_safe0, y); \
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0)); \
	_mm256_store_si256((__m256i * )count_safe0, count);

#define test_diverge_def_noadjust(dx,dy,cmp2,cx,cy,x,y,count,x_safe0,y_safe0,count_safe0,x_safe1,y_safe1,count_safe1) \
	if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] { \
		uint32_t mask = _mm256_movemask_pd(cmp2); \
		double cx_mem[4] __attribute__((__aligned__(32))); \
		double cy_mem[4] __attribute__((__aligned__(32))); \
		double x_mem[4] __attribute__((__aligned__(32))); \
		double y_mem[4] __attribute__((__aligned__(32))); \
		uint64_t count_mem[4] __attribute__((__aligned__(32))); \
		_mm256_store_pd(x_mem, x); \
		_mm256_store_pd(y_mem, y); \
		_mm256_store_si256((__m256i * )count_mem, count); \
		_mm256_store_pd(cx_mem, cx); \
		_mm256_store_pd(cy_mem, cy); \
		while(mask != 0) { \
			uint32_t b = __builtin_ctz(mask); \
			int64_t si=count_safe1[b]; \
			pp[dx[b] + dy[b] * width] = iterations - si; \
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) { \
				cleanup(mse); \
				return; \
			} \
			count_mem[b] = iterations; \
			x_mem[b] = y_mem[b] = 0; \
			mask = mask&~(1U<<b); \
		} \
		x = _mm256_load_pd(x_mem); \
		y = _mm256_load_pd(y_mem); \
		count = _mm256_load_si256((__m256i * )count_mem); \
		cx = _mm256_load_pd(cx_mem); \
		cy = _mm256_load_pd(cy_mem); \
	} \
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0)); \
	_mm256_store_pd(x_safe0, x); \
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0)); \
	_mm256_store_pd(y_safe0, y); \
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0)); \
	_mm256_store_si256((__m256i * )count_safe0, count);

template<class M> inline __attribute__((always_inline)) bool test_diverge(M& mse, iterations_t* pp, uint32_t width, iterations_t iterations, uint32_t* dx, uint32_t* dy,__m256d& cmp2,__m256d& cx,__m256d& cy,__m256d& x,__m256d& y,__m256i& count, double* x_safe0, double* y_safe0, uint64_t* count_safe0, double* x_safe1, double* y_safe1, uint64_t* count_safe1) {
	if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
		uint32_t mask = _mm256_movemask_pd(cmp2);
		double cx_mem[4] __attribute__((__aligned__(32)));
		double cy_mem[4] __attribute__((__aligned__(32)));
		double x_mem[4] __attribute__((__aligned__(32)));
		double y_mem[4] __attribute__((__aligned__(32)));
		uint64_t count_mem[4] __attribute__((__aligned__(32)));
		_mm256_store_pd(x_mem, x);
		_mm256_store_pd(y_mem, y);
		_mm256_store_si256((__m256i * )count_mem, count);
		_mm256_store_pd(cx_mem, cx);
		_mm256_store_pd(cy_mem, cy);
		while(mask != 0) {
			uint32_t b = __builtin_ctz(mask);
			int64_t si=count_safe1[b];
//			last_checks(pp[dx[b] + dy[b] * width],x_safe1+b,y_safe1+b,cx_mem+b,cy_mem+b,iterations,si);
			double sx=x_safe1[b];
			double sy=y_safe1[b];
			double scx=cx_mem[b];
			double scy=cy_mem[b];
			while(si >= 0 && sx * sx + sy * sy <= 4.0) {
				double nx = sx * sx - sy * sy + scx;
				sy = 2.0 * sx * sy + scy;
				sx = nx;
				si--;
			}
			pp[dx[b] + dy[b] * width] = iterations - si;
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
				return true;
			}
			count_mem[b] = iterations;
			x_mem[b] = y_mem[b] = 0;
			//cmp2_mem[b] = 0;
			mask = mask&~(1U<<b);
		}
		x = _mm256_load_pd(x_mem);
		y = _mm256_load_pd(y_mem);
		count = _mm256_load_si256((__m256i * )count_mem);
		//cmp2=_mm256_load_pd(cmp2_mem);
		cx = _mm256_load_pd(cx_mem);
		cy = _mm256_load_pd(cy_mem);
	}
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0));
	_mm256_store_pd(x_safe0, x);
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0));
	_mm256_store_pd(y_safe0, y);
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0));
	_mm256_store_si256((__m256i * )count_safe0, count);
	return false;
}

template<class M> inline __attribute__((always_inline)) bool test_diverge_nou(M& mse, iterations_t* pp, uint32_t width, iterations_t iterations, uint32_t* dx, uint32_t* dy,__m256d& cmp2,__m256d& cx,__m256d& cy,__m256d& x,__m256d& y,__m256i& count, double* x_safe0, double* y_safe0, uint64_t* count_safe0, double* x_safe1, double* y_safe1, uint64_t* count_safe1) {
	if(!_mm256_testz_pd(cmp2, cmp2)) {
		uint32_t mask = _mm256_movemask_pd(cmp2);
		double cx_mem[4] __attribute__((__aligned__(32)));
		double cy_mem[4] __attribute__((__aligned__(32)));
		double x_mem[4] __attribute__((__aligned__(32)));
		double y_mem[4] __attribute__((__aligned__(32)));
		uint64_t count_mem[4] __attribute__((__aligned__(32)));
		_mm256_store_pd(x_mem, x);
		_mm256_store_pd(y_mem, y);
		_mm256_store_si256((__m256i * )count_mem, count);
		_mm256_store_pd(cx_mem, cx);
		_mm256_store_pd(cy_mem, cy);
		while(mask != 0) {
			uint32_t b = __builtin_ctz(mask);
			int64_t si=count_safe1[b];
//			last_checks(pp[dx[b] + dy[b] * width],x_safe1+b,y_safe1+b,cx_mem+b,cy_mem+b,iterations,si);
			double sx=x_safe1[b];
			double sy=y_safe1[b];
			double scx=cx_mem[b];
			double scy=cy_mem[b];
			while(si >= 0 && sx * sx + sy * sy <= 4.0) {
				double nx = sx * sx - sy * sy + scx;
				sy = 2.0 * sx * sy + scy;
				sx = nx;
				si--;
			}
			pp[dx[b] + dy[b] * width] = iterations - si;
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
				return true;
			}
			count_mem[b] = iterations;
			x_mem[b] = y_mem[b] = 0;
			//cmp2_mem[b] = 0;
			mask = mask&~(1U<<b);
		}
		x = _mm256_load_pd(x_mem);
		y = _mm256_load_pd(y_mem);
		count = _mm256_load_si256((__m256i * )count_mem);
		//cmp2=_mm256_load_pd(cmp2_mem);
		cx = _mm256_load_pd(cx_mem);
		cy = _mm256_load_pd(cy_mem);
	}
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0));
	_mm256_store_pd(x_safe0, x);
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0));
	_mm256_store_pd(y_safe0, y);
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0));
	_mm256_store_si256((__m256i * )count_safe0, count);
	return false;
}

template<class M> inline __attribute__((always_inline)) bool test_diverge_noadjust(M& mse, iterations_t* pp, uint32_t width, iterations_t iterations, uint32_t* dx, uint32_t* dy,__m256d& cmp2,__m256d& cx,__m256d& cy,__m256d& x,__m256d& y,__m256i& count, double* x_safe0, double* y_safe0, uint64_t* count_safe0, double* x_safe1, double* y_safe1, uint64_t* count_safe1) {
	if(!_mm256_testz_pd(cmp2, cmp2)) [[unlikely]] {
		uint32_t mask = _mm256_movemask_pd(cmp2);
		double cx_mem[4] __attribute__((__aligned__(32)));
		double cy_mem[4] __attribute__((__aligned__(32)));
		double x_mem[4] __attribute__((__aligned__(32)));
		double y_mem[4] __attribute__((__aligned__(32)));
		uint64_t count_mem[4] __attribute__((__aligned__(32)));
		_mm256_store_pd(x_mem, x);
		_mm256_store_pd(y_mem, y);
		_mm256_store_si256((__m256i * )count_mem, count);
		_mm256_store_pd(cx_mem, cx);
		_mm256_store_pd(cy_mem, cy);
		while(mask != 0) {
			uint32_t b = __builtin_ctz(mask);
			int64_t si=count_safe1[b];
//			last_checks(pp[dx[b] + dy[b] * width],x_safe1+b,y_safe1+b,cx_mem+b,cy_mem+b,iterations,si);
			pp[dx[b] + dy[b] * width] = iterations - si;
			if(!mse.get_next_point(cx_mem[b], cy_mem[b], dx[b], dy[b])) {
				return true;
			}
			count_mem[b] = iterations;
			x_mem[b] = y_mem[b] = 0;
			//cmp2_mem[b] = 0;
			mask = mask&~(1U<<b);
		}
		x = _mm256_load_pd(x_mem);
		y = _mm256_load_pd(y_mem);
		count = _mm256_load_si256((__m256i * )count_mem);
		//cmp2=_mm256_load_pd(cmp2_mem);
		cx = _mm256_load_pd(cx_mem);
		cy = _mm256_load_pd(cy_mem);
	}
	_mm256_store_pd(x_safe1,_mm256_load_pd(x_safe0));
	_mm256_store_pd(x_safe0, x);
	_mm256_store_pd(y_safe1,_mm256_load_pd(y_safe0));
	_mm256_store_pd(y_safe0, y);
	_mm256_store_si256((__m256i * )count_safe1,_mm256_load_si256((__m256i * )count_safe0));
	_mm256_store_si256((__m256i * )count_safe0, count);
	return false;
}

inline __attribute__((always_inline)) void iterate3(__m256d& s1_x, __m256d& s2_x, __m256d& s3_x, __m256d& s1_y, __m256d& s2_y, __m256d& s3_y,
					 __m256d s1_cx, __m256d s2_cx, __m256d s3_cx, __m256d s1_cy, __m256d s2_cy, __m256d s3_cy, __m256i add_one_exp) {
	__m256d s1b_temp_y = s1_y;
	__m256d s2b_temp_y = s2_y;
	__m256d s3b_temp_y = s3_y;
	// s1_x_squared = s1_x * s1_x;
	__m256d s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
	__m256d s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
	__m256d s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
	// s1_y = 2 * s1_y * s1_x + s1_cy;
	s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
	s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
	s3_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s3_x, add_one_exp), s3_y, s3_cy);
	// s1_x = s1_x_squared - s1_y_squared + s1_cx;
	s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
	s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));
	s3_x = _mm256_sub_pd(s3_x_squared, _mm256_fmsub_pd(s3b_temp_y, s3b_temp_y, s3_cx));
}

template<class M> void Mandelbrot::render_avx_i3_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(3);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
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
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
			cleanup(mse);
			return;
		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate3(s1_x, s2_x, s3_x, s1_y, s2_y, s3_y, s1_cx, s2_cx, s3_cx, s1_cy, s2_cy, s3_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		iterate3(s1_x, s2_x, s3_x, s1_y, s2_y, s3_y, s1_cx, s2_cx, s3_cx, s1_cy, s2_cy, s3_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		iterate3(s1_x, s2_x, s3_x, s1_y, s2_y, s3_y, s1_cx, s2_cx, s3_cx, s1_cy, s2_cy, s3_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		iterate3(s1_x, s2_x, s3_x, s1_y, s2_y, s3_y, s1_cx, s2_cx, s3_cx, s1_cy, s2_cy, s3_cy, add_one_exp);
	}
}

inline __attribute__((always_inline)) void iterate4(__m256d& s1_x, __m256d& s2_x, __m256d& s3_x, __m256d& s4_x, __m256d& s1_y, __m256d& s2_y, __m256d& s3_y, __m256d& s4_y,
					 __m256d s1_cx, __m256d s2_cx, __m256d s3_cx, __m256d s4_cx, __m256d s1_cy, __m256d s2_cy, __m256d s3_cy, __m256d s4_cy, __m256i add_one_exp) {
	__m256d s1b_temp_y = s1_y;
	__m256d s2b_temp_y = s2_y;
	__m256d s3b_temp_y = s3_y;
	__m256d s4b_temp_y = s4_y;
	// s1_x_squared = s1_x * s1_x;
	__m256d s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
	__m256d s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
	__m256d s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
	__m256d s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
	// s1_y = 2 * s1_y * s1_x + s1_cy;
	s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
	s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
	s3_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s3_x, add_one_exp), s3_y, s3_cy);
	s4_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s4_x, add_one_exp), s4_y, s4_cy);
	// s1_x = s1_x_squared - s1_y_squared + s1_cx;
	s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
	s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));
	s3_x = _mm256_sub_pd(s3_x_squared, _mm256_fmsub_pd(s3b_temp_y, s3b_temp_y, s3_cx));
	s4_x = _mm256_sub_pd(s4_x_squared, _mm256_fmsub_pd(s4b_temp_y, s4b_temp_y, s4_cx));
}

template<class M> void Mandelbrot::render_avx_i4_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256i s1_count = _mm256_set1_epi64x(iterations);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
			cleanup(mse);
			return;
		}
		// s1_x_squared = s1_x * s1_x;
		__m256d s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		__m256d s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		__m256d s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		__m256d s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		__m256d s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		__m256d s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		__m256d s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		__m256d s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		__m256d s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		__m256d s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		__m256d s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		__m256d s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

template<class M> void Mandelbrot::render_avx_i4_u_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256d s4_x_squared = _mm256_setzero_pd();
	__m256d s4_y_squared = _mm256_setzero_pd();
	__m256d s4_mag = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		test_diverge_def(s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)
		test_diverge_def(s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)
		test_diverge_def(s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)
		test_diverge_def(s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)
// 		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

template<class M> void Mandelbrot::render_avx_i4_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256i s1_count = _mm256_set1_epi64x(iterations);
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		if(test_diverge_nou(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_nou(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_nou(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_nou(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
			cleanup(mse);
			return;
		}
		// s1_x_squared = s1_x * s1_x;
		__m256d s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		__m256d s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		__m256d s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		__m256d s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		__m256d s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		__m256d s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		__m256d s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		__m256d s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		__m256d s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		__m256d s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		__m256d s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		__m256d s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

template<class M> void Mandelbrot::render_avx_i4_tinc_addint_fast_by4_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256d s4_x_squared = _mm256_setzero_pd();
	__m256d s4_y_squared = _mm256_setzero_pd();
	__m256d s4_mag = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		test_diverge_def_nou(s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)
		test_diverge_def_nou(s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)
		test_diverge_def_nou(s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)
		test_diverge_def_nou(s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)
// 		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

template<class M> void Mandelbrot::render_avx_i4_u_tinc_addint_fast_by4_noadjust(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256d s4_x_squared = _mm256_setzero_pd();
	__m256d s4_y_squared = _mm256_setzero_pd();
	__m256d s4_mag = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
// 		test_diverge_def(s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)
// 		test_diverge_def(s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)
// 		test_diverge_def(s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)
// 		test_diverge_def(s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)
		if(test_diverge_noadjust(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_noadjust(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_noadjust(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
			cleanup(mse);
			return;
		}
		if(test_diverge_noadjust(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
			cleanup(mse);
			return;
		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

template<class M> void Mandelbrot::render_avx_i4_u_tinc_addint_fast_by4_noadjust_macro(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256d s4_x_squared = _mm256_setzero_pd();
	__m256d s4_y_squared = _mm256_setzero_pd();
	__m256d s4_mag = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		test_diverge_def_noadjust(s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)
		test_diverge_def_noadjust(s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)
		test_diverge_def_noadjust(s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)
		test_diverge_def_noadjust(s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)
// 		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		iterate4(s1_x, s2_x, s3_x, s4_x, s1_y, s2_y, s3_y, s4_y, s1_cx, s2_cx, s3_cx, s4_cx, s1_cy, s2_cy, s3_cy, s4_cy, add_one_exp);
	}
}

inline __attribute__((always_inline)) void iterate5(__m256d& s1_x, __m256d& s2_x, __m256d& s3_x, __m256d& s4_x, __m256d& s5_x, __m256d& s1_y, __m256d& s2_y, __m256d& s3_y, __m256d& s4_y, __m256d& s5_y,
					 __m256d s1_cx, __m256d s2_cx, __m256d s3_cx, __m256d s4_cx, __m256d s5_cx, __m256d s1_cy, __m256d s2_cy, __m256d s3_cy, __m256d s4_cy, __m256d s5_cy, __m256i add_one_exp) {
	__m256d s1b_temp_y = s1_y;
	__m256d s2b_temp_y = s2_y;
	__m256d s3b_temp_y = s3_y;
	__m256d s4b_temp_y = s4_y;
	__m256d s5b_temp_y = s5_y;
	// s1_x_squared = s1_x * s1_x;
	__m256d s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
	__m256d s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
	__m256d s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
	__m256d s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
	__m256d s5_x_squared = _mm256_mul_pd(s5_x, s5_x);
	// s1_y = 2 * s1_y * s1_x + s1_cy;
	s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_x, add_one_exp), s1_y, s1_cy);
	s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_x, add_one_exp), s2_y, s2_cy);
	s3_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s3_x, add_one_exp), s3_y, s3_cy);
	s4_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s4_x, add_one_exp), s4_y, s4_cy);
	s5_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s5_x, add_one_exp), s5_y, s5_cy);
	// s1_x = s1_x_squared - s1_y_squared + s1_cx;
	s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1b_temp_y, s1b_temp_y, s1_cx));
	s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2b_temp_y, s2b_temp_y, s2_cx));
	s3_x = _mm256_sub_pd(s3_x_squared, _mm256_fmsub_pd(s3b_temp_y, s3b_temp_y, s3_cx));
	s4_x = _mm256_sub_pd(s4_x_squared, _mm256_fmsub_pd(s4b_temp_y, s4b_temp_y, s4_cx));
	s5_x = _mm256_sub_pd(s5_x_squared, _mm256_fmsub_pd(s5b_temp_y, s5b_temp_y, s5_cx));
}

template<class M> void Mandelbrot::render_avx_i5_u_tinc_addint_fast_by4(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
	__m256d s1_cmp_with_four = _mm256_setzero_pd();
	__m256d s1_cmp2 = _mm256_setzero_pd();
	__m256d s2_x = _mm256_setzero_pd();
	__m256d s2_y = _mm256_setzero_pd();
	__m256d s2_x_squared = _mm256_setzero_pd();
	__m256d s2_y_squared = _mm256_setzero_pd();
	__m256d s2_mag = _mm256_setzero_pd();
	__m256i s2_count = _mm256_set1_epi64x(iterations);
	__m256d s2_cmp_with_four = _mm256_setzero_pd();
	__m256d s2_cmp2 = _mm256_setzero_pd();
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations);
	__m256d s3_cmp_with_four = _mm256_setzero_pd();
	__m256d s3_cmp2 = _mm256_setzero_pd();
	__m256d s4_x = _mm256_setzero_pd();
	__m256d s4_y = _mm256_setzero_pd();
	__m256d s4_x_squared = _mm256_setzero_pd();
	__m256d s4_y_squared = _mm256_setzero_pd();
	__m256d s4_mag = _mm256_setzero_pd();
	__m256i s4_count = _mm256_set1_epi64x(iterations);
	__m256d s4_cmp_with_four = _mm256_setzero_pd();
	__m256d s4_cmp2 = _mm256_setzero_pd();
	__m256d s5_x = _mm256_setzero_pd();
	__m256d s5_y = _mm256_setzero_pd();
	__m256d s5_x_squared = _mm256_setzero_pd();
	__m256d s5_y_squared = _mm256_setzero_pd();
	__m256d s5_mag = _mm256_setzero_pd();
	__m256i s5_count = _mm256_set1_epi64x(iterations);
	__m256d s5_cmp_with_four = _mm256_setzero_pd();
	__m256d s5_cmp2 = _mm256_setzero_pd();
	__m256i one_int64 = _mm256_set1_epi64x(4);
	__m256d four_double = _mm256_set1_pd(4.0);
	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
	uint32_t s3_dx[4] = {}, s3_dy[4] = {};
	uint32_t s4_dx[4] = {}, s4_dy[4] = {};
	uint32_t s5_dx[4] = {}, s5_dy[4] = {};
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
	mse.get_next_point(cx_mem[0], cy_mem[0], s4_dx[0], s4_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s4_dx[1], s4_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s4_dx[2], s4_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s4_dx[3], s4_dy[3]);
	__m256d s4_cx = *(__m256d * )cx_mem;
	__m256d s4_cy = *(__m256d * )cy_mem;
	mse.get_next_point(cx_mem[0], cy_mem[0], s5_dx[0], s5_dy[0]);
	mse.get_next_point(cx_mem[1], cy_mem[1], s5_dx[1], s5_dy[1]);
	mse.get_next_point(cx_mem[2], cy_mem[2], s5_dx[2], s5_dy[2]);
	mse.get_next_point(cx_mem[3], cy_mem[3], s5_dx[3], s5_dy[3]);
	__m256d s5_cx = *(__m256d * )cx_mem;
	__m256d s5_cy = *(__m256d * )cy_mem;
	double s1_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s5_x_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe0[4] __attribute__((__aligned__(32))) = {};
	double s5_y_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe0[4] __attribute__((__aligned__(32))) = {};
	uint64_t s5_count_safe0[4] __attribute__((__aligned__(32))) = {};
	double s1_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s5_x_safe1[4] __attribute__((__aligned__(32))) = {};
	double s1_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s2_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s3_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s4_y_safe1[4] __attribute__((__aligned__(32))) = {};
	double s5_y_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s1_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s2_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s3_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s4_count_safe1[4] __attribute__((__aligned__(32))) = {};
	uint64_t s5_count_safe1[4] __attribute__((__aligned__(32))) = {};
	while(true) {
		test_diverge_def(s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)
		test_diverge_def(s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)
		test_diverge_def(s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)
		test_diverge_def(s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)
		test_diverge_def(s5_dx,s5_dy,s5_cmp2,s5_cx,s5_cy,s5_x,s5_y,s5_count,s5_x_safe0,s5_y_safe0,s5_count_safe0,s5_x_safe1,s5_y_safe1,s5_count_safe1)
// 		if(test_diverge(mse,pp,width,iterations,s1_dx,s1_dy,s1_cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_count,s1_x_safe0,s1_y_safe0,s1_count_safe0,s1_x_safe1,s1_y_safe1,s1_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s2_dx,s2_dy,s2_cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_count,s2_x_safe0,s2_y_safe0,s2_count_safe0,s2_x_safe1,s2_y_safe1,s2_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s3_dx,s3_dy,s3_cmp2,s3_cx,s3_cy,s3_x,s3_y,s3_count,s3_x_safe0,s3_y_safe0,s3_count_safe0,s3_x_safe1,s3_y_safe1,s3_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s4_dx,s4_dy,s4_cmp2,s4_cx,s4_cy,s4_x,s4_y,s4_count,s4_x_safe0,s4_y_safe0,s4_count_safe0,s4_x_safe1,s4_y_safe1,s4_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		if(test_diverge(mse,pp,width,iterations,s5_dx,s5_dy,s5_cmp2,s5_cx,s5_cy,s5_x,s5_y,s5_count,s5_x_safe0,s5_y_safe0,s5_count_safe0,s5_x_safe1,s5_y_safe1,s5_count_safe1)) {
// 			cleanup(mse);
// 			return;
// 		}
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		s4_x_squared = _mm256_mul_pd(s4_x, s4_x);
		s5_x_squared = _mm256_mul_pd(s5_x, s5_x);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		s4_y_squared = _mm256_mul_pd(s4_y, s4_y);
		s5_y_squared = _mm256_mul_pd(s5_y, s5_y);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		iterate5(s1_x, s2_x, s3_x, s4_x, s5_x, s1_y, s2_y, s3_y, s4_y, s5_y, s1_cx, s2_cx, s3_cx, s4_cx, s5_cx, s1_cy, s2_cy, s3_cy, s4_cy, s5_cy, add_one_exp);
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		s4_mag = _mm256_add_pd(s4_x_squared, s4_y_squared);
		s5_mag = _mm256_add_pd(s5_x_squared, s5_y_squared);
		iterate5(s1_x, s2_x, s3_x, s4_x, s5_x, s1_y, s2_y, s3_y, s4_y, s5_y, s1_cx, s2_cx, s3_cx, s4_cx, s5_cx, s1_cy, s2_cy, s3_cy, s4_cy, s5_cy, add_one_exp);
		s1_cmp_with_four=_mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS);
		s2_cmp_with_four=_mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS);
		s3_cmp_with_four=_mm256_cmp_pd(s3_mag, four_double, _CMP_GT_OS);
		s4_cmp_with_four=_mm256_cmp_pd(s4_mag, four_double, _CMP_GT_OS);
		s5_cmp_with_four=_mm256_cmp_pd(s5_mag, four_double, _CMP_GT_OS);
		iterate5(s1_x, s2_x, s3_x, s4_x, s5_x, s1_y, s2_y, s3_y, s4_y, s5_y, s1_cx, s2_cx, s3_cx, s4_cx, s5_cx, s1_cy, s2_cy, s3_cy, s4_cy, s5_cy, add_one_exp);
		s1_cmp2 = _mm256_or_pd((__m256d)s1_count, s1_cmp_with_four);
		s2_cmp2 = _mm256_or_pd((__m256d)s2_count, s2_cmp_with_four);
		s3_cmp2 = _mm256_or_pd((__m256d)s3_count, s3_cmp_with_four);
		s4_cmp2 = _mm256_or_pd((__m256d)s4_count, s4_cmp_with_four);
		s5_cmp2 = _mm256_or_pd((__m256d)s5_count, s5_cmp_with_four);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		s4_count = _mm256_sub_epi64(s4_count, one_int64);
		s5_count = _mm256_sub_epi64(s5_count, one_int64);
		iterate5(s1_x, s2_x, s3_x, s4_x, s5_x, s1_y, s2_y, s3_y, s4_y, s5_y, s1_cx, s2_cx, s3_cx, s4_cx, s5_cx, s1_cy, s2_cy, s3_cy, s4_cy, s5_cy, add_one_exp);
	}
}

// template<class M> void Mandelbrot::render_avx_sheeprace2_u_tinc_addint_fast2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
// 	if(render_width == 0) {
// 		render_width = width - render_x;
// 	}
// 	if(render_height == 0) {
// 		render_height = height - render_y;
// 	}
// 	M mse(p, width, height, render_x, render_y, render_width, render_height, xs, ys, inc, iterations);
// 	iterations_t* pp = mse.p;
// 	reset(pp, width, render_width, render_height);
// 	iterations--;
// 	__m256d s1_x = _mm256_setzero_pd();
// 	__m256d s1_y = _mm256_setzero_pd();
// 	__m256d s1_x_squared = _mm256_setzero_pd();
// 	__m256d s1_y_squared = _mm256_setzero_pd();
// 	__m256d s1_mag = _mm256_setzero_pd();
// 	__m256i s1_count = _mm256_set1_epi64x(iterations + 2);
// 	__m256d s2_x = _mm256_setzero_pd();
// 	__m256d s2_y = _mm256_setzero_pd();
// 	__m256d s2_x_squared = _mm256_setzero_pd();
// 	__m256d s2_y_squared = _mm256_setzero_pd();
// 	__m256d s2_mag = _mm256_setzero_pd();
// 	__m256i s2_count = _mm256_set1_epi64x(iterations + 2);
// 	__m256i one_int64 = _mm256_set1_epi64x(1);
// 	__m256d four_double = _mm256_set1_pd(4.0);
// 	__m256i add_one_exp = _mm256_set1_epi64x(0x10000000000000);
// 	uint32_t s1_dx[4] = {}, s1_dy[4] = {};
// 	uint32_t s2_dx[4] = {}, s2_dy[4] = {};
// 	double cx_mem[4] __attribute__((__aligned__(32)));
// 	double cy_mem[4] __attribute__((__aligned__(32)));
// 	mse.get_next_point(cx_mem[0], cy_mem[0], s1_dx[0], s1_dy[0]);
// 	mse.get_next_point(cx_mem[1], cy_mem[1], s1_dx[1], s1_dy[1]);
// 	mse.get_next_point(cx_mem[2], cy_mem[2], s1_dx[2], s1_dy[2]);
// 	mse.get_next_point(cx_mem[3], cy_mem[3], s1_dx[3], s1_dy[3]);
// 	__m256d s1_cx = *(__m256d * )cx_mem;
// 	__m256d s1_cy = *(__m256d * )cy_mem;
// 	mse.get_next_point(cx_mem[0], cy_mem[0], s2_dx[0], s2_dy[0]);
// 	mse.get_next_point(cx_mem[1], cy_mem[1], s2_dx[1], s2_dy[1]);
// 	mse.get_next_point(cx_mem[2], cy_mem[2], s2_dx[2], s2_dy[2]);
// 	mse.get_next_point(cx_mem[3], cy_mem[3], s2_dx[3], s2_dy[3]);
// 	__m256d s2_cx = *(__m256d * )cx_mem;
// 	__m256d s2_cy = *(__m256d * )cy_mem;
// 	while(true) {
// 		__m256d cmp2 = _mm256_or_pd((__m256d)s1_count, _mm256_cmp_pd(s1_mag, four_double, _CMP_GT_OS));
// 		if(test_inline2u(mse,pp,width,iterations,s1_dx,s1_dy,cmp2,s1_cx,s1_cy,s1_x,s1_y,s1_x_squared,s1_y_squared,s1_count)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
// 		if(test_inline2u(mse,pp,width,iterations,s2_dx,s2_dy,cmp2,s2_cx,s2_cy,s2_x,s2_y,s2_x_squared,s2_y_squared,s2_count)) {
// 			cleanup(mse);
// 			return;
// 		}
// 		// s1_mag = s1_x_squared + s1_y_squared;
// 		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
// 		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
// 		// Decrement counter
// 		s1_count = _mm256_sub_epi64(s1_count, one_int64);
// 		s2_count = _mm256_sub_epi64(s2_count, one_int64);
// 		// make a temp copy of y
// 		__m256d s1_temp_x = s1_x;
// 		__m256d s2_temp_x = s2_x;
// 		// s1_x_squared = s1_x * s1_x;
// 		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
// 		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
// 		// s1_y_squared = s1_y * s1_y;
// 		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
// 		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
// 		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
// 		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_fmsub_pd(s1_y, s1_y, s1_cx));
// 		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_fmsub_pd(s2_y, s2_y, s2_cx));
// 		// s1_y = 2 * s1_y * s1_x + s1_cy;
// 		s1_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s1_temp_x, add_one_exp), s1_y, s1_cy);
// 		s2_y = _mm256_fmadd_pd((__m256d)_mm256_add_epi64((__m256i)s2_temp_x, add_one_exp), s2_y, s2_cy);
// 	}
// }

template<class M> void Mandelbrot::render_avx_sheeprace2_expect(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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
		if(__builtin_expect(!_mm256_testz_pd(cmp2, cmp2),0)) {
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
		cmp2 = _mm256_or_pd((__m256d)s2_count, _mm256_cmp_pd(s2_mag, four_double, _CMP_GT_OS));
		if(__builtin_expect(!_mm256_testz_pd(cmp2, cmp2),0)) {
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
				s2_count_mem[b] = iterations + 2;
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

template<class M> void Mandelbrot::render_avx_sheeprace2_icmp2(iterations_t iterations, uint32_t render_x, uint32_t render_y, uint32_t render_width, uint32_t render_height) {
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

		t1 = _mm256_or_pd((__m256d)s2_count, _mm256_cmpgt_epi64(s2_mag, four_double));
		TEST(s2_x, s2_y, s2_cx, s2_cy, s2_dx, s2_dy, s2_count);

		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s1_mag   = _mm256_fmadd_pd(s1_x, s1_x, s1_y);
		t1       = _mm256_add_pd(s1_x, s1_x);
		t1       = _mm256_fmadd_pd(t1, s1_y, s1_cy);
		s1_y     = _mm256_mul_pd(s1_y, s1_y);
		t2       = _mm256_fmsub_pd(s1_x, s1_x, s1_y);
		s1_x     = _mm256_add_pd(t2, s1_cx);
		s1_y     = t1;

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

template<class M> __attribute__((noinline)) bool update_avx_stream(M& mse, uint32_t index, uint32_t mask,__m256d& s1_cx,__m256d& s1_cy,__m256d& s1_x,__m256d& s1_y,__m256d& s1_x_squared,__m256d& s1_y_squared,__m256i& s1_count) {
	double cx_mem[4] __attribute__((__aligned__(32)));
	double cy_mem[4] __attribute__((__aligned__(32)));
	double s1_x_mem[4] __attribute__((__aligned__(32)));
	double s1_y_mem[4] __attribute__((__aligned__(32)));
	uint64_t s1_count_mem[4] __attribute__((__aligned__(32)));
	double s1_x_squared_mem[4] __attribute__((__aligned__(32)));
	double s1_y_squared_mem[4] __attribute__((__aligned__(32)));
	_mm256_store_pd(cx_mem, s1_cx);
	_mm256_store_pd(cy_mem, s1_cy);
	_mm256_store_pd(s1_x_mem, s1_x);
	_mm256_store_pd(s1_y_mem, s1_y);
	_mm256_store_si256((__m256i * )s1_count_mem, s1_count);
	_mm256_store_pd(s1_x_squared_mem, s1_x_squared);
	_mm256_store_pd(s1_y_squared_mem, s1_y_squared);
	while(mask != 0) {
		uint32_t b = __builtin_ctz(mask);
		mse.p[mse.dx[index][b] + mse.dy[index][b] * mse.stored_width] = mse.stored_iterations - s1_count_mem[b];
		if(!mse.get_next_point(cx_mem[b], cy_mem[b], mse.dx[index][b], mse.dy[index][b])) {
			return false;
		}
		s1_count_mem[b] = mse.stored_iterations + 2;
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
	__m256d s3_x = _mm256_setzero_pd();
	__m256d s3_y = _mm256_setzero_pd();
	__m256d s3_x_squared = _mm256_setzero_pd();
	__m256d s3_y_squared = _mm256_setzero_pd();
	__m256d s3_mag = _mm256_setzero_pd();
	__m256i s3_count = _mm256_set1_epi64x(iterations + 2);
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
				s2_count_mem[b] = iterations + 2;
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
				s3_count_mem[b] = iterations + 2;
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
		// s1_mag = s1_x_squared + s1_y_squared;
		s1_mag = _mm256_add_pd(s1_x_squared, s1_y_squared);
		s2_mag = _mm256_add_pd(s2_x_squared, s2_y_squared);
		s3_mag = _mm256_add_pd(s3_x_squared, s3_y_squared);
		// Decrement counter
		s1_count = _mm256_sub_epi64(s1_count, one_int64);
		s2_count = _mm256_sub_epi64(s2_count, one_int64);
		s3_count = _mm256_sub_epi64(s3_count, one_int64);
		// s1_y_squared = s1_y * s1_y;
		s1_y_squared = _mm256_mul_pd(s1_y, s1_y);
		s2_y_squared = _mm256_mul_pd(s2_y, s2_y);
		s3_y_squared = _mm256_mul_pd(s3_y, s3_y);
		// s1_x_squared = s1_x * s1_x;
		s1_x_squared = _mm256_mul_pd(s1_x, s1_x);
		s2_x_squared = _mm256_mul_pd(s2_x, s2_x);
		s3_x_squared = _mm256_mul_pd(s3_x, s3_x);
		// s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_y = _mm256_fmadd_pd(_mm256_add_pd(s1_x, s1_x), s1_y, s1_cy);
		s2_y = _mm256_fmadd_pd(_mm256_add_pd(s2_x, s2_x), s2_y, s2_cy);
		s3_y = _mm256_fmadd_pd(_mm256_add_pd(s3_x, s3_x), s3_y, s3_cy);
		// s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s1_x = _mm256_sub_pd(s1_x_squared, _mm256_sub_pd(s1_y_squared, s1_cx));
		s2_x = _mm256_sub_pd(s2_x_squared, _mm256_sub_pd(s2_y_squared, s2_cx));
		s3_x = _mm256_sub_pd(s3_x_squared, _mm256_sub_pd(s3_y_squared, s3_cx));
	}
}

