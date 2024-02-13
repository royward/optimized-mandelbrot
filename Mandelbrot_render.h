#pragma once

#include <immintrin.h>

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


