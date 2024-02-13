#include <cstdint>
#include <string>
#include <iostream>
#include "Mandelbrot.h"
#include "Mandelbrot_render.h"
#include "MandelbrotStateEngineSimple.h"

uint32_t mandelbrot_point_fast_math(double cx, double cy, uint32_t m) {
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

uint32_t mandelbrot_point_fast_math_decrement(double cx, double cy, uint32_t m) {
	uint32_t count=m;
	double x=0,y=0;
	while(count!=0 && x*x+y*y<=4.0) {
		double nx=x*x-y*y+cx;
		y=2*x*y+cy;
		x=nx;
		count--;
	}
	return m-count;
}

iterations_t mandelbrot_sheeprace(double cx, double cy, iterations_t m) {
	double x = 0, y = 0, x_squared = 0, y_squared = 0, mag_squared = 0;
	iterations_t count = m + 2;
	while(count != 0 && mag_squared <= 4.0) {
		mag_squared = x_squared + y_squared;
		y_squared = y * y;
		x_squared = x * x;
		double newx = x_squared - y_squared + cx;
		y = 2 * y * x + cy;
		x = newx;
		count--;
	}
	return m - count;
}

void mandelbrot2_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r) {
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0, s1_mag_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0, s2_mag_squared = 0;
	iterations_t count = m + 2;
	while(count != 0) {
		if(s2_mag_squared > 4.0) {
			// write out stream 1
			*s2_r = m - count;
			// now only stream 0 left
			goto single_point;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s2_r;
			s1_x = s2_x;
			s1_y = s2_y;
			s1_cx = s2_cx;
			s1_cy = s2_cy;
			s1_x_squared = s2_x_squared;
			s1_y_squared = s2_y_squared;
			s1_mag_squared = s2_mag_squared;
			// now only stream 0 left
			goto single_point;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
	}
	*s1_r = m;
	*s2_r = m;
	return;
single_point:
	while(count != 0 && s1_mag_squared <= 4.0) {
		s1_mag_squared = s1_x_squared + s1_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s1_x_squared = s1_x * s1_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
	}
	*s1_r = m - count;
}

void mandelbrot3_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, double s3_cx, double s3_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r, iterations_t* s3_r) {
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0, s1_mag_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0, s2_mag_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0, s3_mag_squared = 0;
	iterations_t count = m + 2;
	while(count != 0) {
		if(s3_mag_squared > 4.0) {
			// write out stream 1
			*s3_r = m - count;
			// now only stream 0 left
			goto two_points;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s3_r;
			s1_x = s3_x;
			s1_y = s3_y;
			s1_cx = s3_cx;
			s1_cy = s3_cy;
			s1_x_squared = s3_x_squared;
			s1_y_squared = s3_y_squared;
			s1_mag_squared = s3_mag_squared;
			// now only stream 0 left
			goto two_points;
		}
		if(s2_mag_squared > 4.0) {
			// write out stream 0
			*s2_r = m - count;
			// transfer stream 1 to stream 0
			s2_r = s3_r;
			s2_x = s3_x;
			s2_y = s3_y;
			s2_cx = s3_cx;
			s2_cy = s3_cy;
			s2_x_squared = s3_x_squared;
			s2_y_squared = s3_y_squared;
			s2_mag_squared = s3_mag_squared;
			// now only stream 0 left
			goto two_points;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		s3_mag_squared = s3_x_squared + s3_y_squared;
		count--;
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
	*s1_r = m;
	*s2_r = m;
	*s3_r = m;
	return;
two_points:
	while(count != 0) {
		if(s2_mag_squared > 4.0) {
			// write out stream 1
			*s2_r = m - count;
			// now only stream 0 left
			goto single_point;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s2_r;
			s1_x = s2_x;
			s1_y = s2_y;
			s1_cx = s2_cx;
			s1_cy = s2_cy;
			s1_x_squared = s2_x_squared;
			s1_y_squared = s2_y_squared;
			s1_mag_squared = s2_mag_squared;
			// now only stream 0 left
			goto single_point;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
	}
	*s1_r = m;
	*s2_r = m;
	return;
single_point:
	while(count != 0 && s1_mag_squared <= 4.0) {
		s1_mag_squared = s1_x_squared + s1_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s1_x_squared = s1_x * s1_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
	}
	*s1_r = m - count;
}

void mandelbrot4_sheeprace(double s1_cx, double s1_cy, double s2_cx, double s2_cy, double s3_cx, double s3_cy, double s4_cx, double s4_cy, uint32_t m, iterations_t* s1_r, iterations_t* s2_r, iterations_t* s3_r, iterations_t* s4_r) {
	double s1_x = 0, s1_y = 0, s1_x_squared = 0, s1_y_squared = 0, s1_mag_squared = 0;
	double s2_x = 0, s2_y = 0, s2_x_squared = 0, s2_y_squared = 0, s2_mag_squared = 0;
	double s3_x = 0, s3_y = 0, s3_x_squared = 0, s3_y_squared = 0, s3_mag_squared = 0;
	double s4_x = 0, s4_y = 0, s4_x_squared = 0, s4_y_squared = 0, s4_mag_squared = 0;
	iterations_t count = m + 2;
	while(count != 0) {
		if(s4_mag_squared > 4.0) {
			// write out stream 1
			*s4_r = m - count;
			// now only stream 0 left
			goto three_points;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s4_r;
			s1_x = s4_x;
			s1_y = s4_y;
			s1_cx = s4_cx;
			s1_cy = s4_cy;
			s1_x_squared = s4_x_squared;
			s1_y_squared = s4_y_squared;
			s1_mag_squared = s4_mag_squared;
			// now only stream 0 left
			goto three_points;
		}
		if(s2_mag_squared > 4.0) {
			// write out stream 0
			*s2_r = m - count;
			// transfer stream 1 to stream 0
			s2_r = s4_r;
			s2_x = s4_x;
			s2_y = s4_y;
			s2_cx = s4_cx;
			s2_cy = s4_cy;
			s2_x_squared = s4_x_squared;
			s2_y_squared = s4_y_squared;
			s2_mag_squared = s4_mag_squared;
			// now only stream 0 left
			goto three_points;
		}
		if(s3_mag_squared > 4.0) {
			// write out stream 0
			*s3_r = m - count;
			// transfer stream 1 to stream 0
			s3_r = s4_r;
			s3_x = s4_x;
			s3_y = s4_y;
			s3_cx = s4_cx;
			s3_cy = s4_cy;
			s3_x_squared = s4_x_squared;
			s3_y_squared = s4_y_squared;
			s3_mag_squared = s4_mag_squared;
			// now only stream 0 left
			goto three_points;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		s3_mag_squared = s3_x_squared + s3_y_squared;
		s4_mag_squared = s4_x_squared + s4_y_squared;
		count--;
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
	*s1_r = m;
	*s2_r = m;
	*s3_r = m;
	*s4_r = m;
	return;
three_points:
	while(count != 0) {
		if(s3_mag_squared > 4.0) {
			// write out stream 1
			*s3_r = m - count;
			// now only stream 0 left
			goto two_points;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s3_r;
			s1_x = s3_x;
			s1_y = s3_y;
			s1_cx = s3_cx;
			s1_cy = s3_cy;
			s1_x_squared = s3_x_squared;
			s1_y_squared = s3_y_squared;
			s1_mag_squared = s3_mag_squared;
			// now only stream 0 left
			goto two_points;
		}
		if(s2_mag_squared > 4.0) {
			// write out stream 0
			*s2_r = m - count;
			// transfer stream 1 to stream 0
			s2_r = s3_r;
			s2_x = s3_x;
			s2_y = s3_y;
			s2_cx = s3_cx;
			s2_cy = s3_cy;
			s2_x_squared = s3_x_squared;
			s2_y_squared = s3_y_squared;
			s2_mag_squared = s3_mag_squared;
			// now only stream 0 left
			goto two_points;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		s3_mag_squared = s3_x_squared + s3_y_squared;
		count--;
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
	*s1_r = m;
	*s2_r = m;
	*s3_r = m;
	return;
two_points:
	while(count != 0) {
		if(s2_mag_squared > 4.0) {
			// write out stream 1
			*s2_r = m - count;
			// now only stream 0 left
			goto single_point;
		}
		if(s1_mag_squared > 4.0) {
			// write out stream 0
			*s1_r = m - count;
			// transfer stream 1 to stream 0
			s1_r = s2_r;
			s1_x = s2_x;
			s1_y = s2_y;
			s1_cx = s2_cx;
			s1_cy = s2_cy;
			s1_x_squared = s2_x_squared;
			s1_y_squared = s2_y_squared;
			s1_mag_squared = s2_mag_squared;
			// now only stream 0 left
			goto single_point;
		}
		s1_mag_squared = s1_x_squared + s1_y_squared;
		s2_mag_squared = s2_x_squared + s2_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s2_y_squared = s2_y * s2_y;
		s1_x_squared = s1_x * s1_x;
		s2_x_squared = s2_x * s2_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s2_y = 2 * s2_y * s2_x + s2_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
		s2_x = s2_x_squared - s2_y_squared + s2_cx;
	}
	*s1_r = m;
	*s2_r = m;
	return;
single_point:
	while(count != 0 && s1_mag_squared <= 4.0) {
		s1_mag_squared = s1_x_squared + s1_y_squared;
		count--;
		s1_y_squared = s1_y * s1_y;
		s1_x_squared = s1_x * s1_x;
		s1_y = 2 * s1_y * s1_x + s1_cy;
		s1_x = s1_x_squared - s1_y_squared + s1_cx;
	}
	*s1_r = m - count;
}

uint64_t time_elapsed_ns();

