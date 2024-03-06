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

#include <cstdint>
#include <vector>

//#define EDGE_FOLLOW_QUEUE 1

const iterations_t MAX = std::numeric_limits<iterations_t>::max();

template <typename T> void fill(T* p, uint32_t stride, uint32_t width, uint32_t height) {
	for(uint32_t j = 1;j<height - 1;j++) {
		T* py = &p[stride * j];
		T val = py[0];
		for(uint32_t i = 1;i<width - 1;i++) {
			if(py[i] == 0) {
				py[i] = val;
			} else {
				val = py[i];
			}
		}
	}
}

inline void setup_point(double xs, double ys, double inc, double& px, double& py, uint32_t& x, uint32_t& y, uint32_t w, uint32_t h) {
	x = w;
	y = h;
	px = xs + w * inc;
	py = ys - h * inc;
}

struct Point {
	Point(uint16_t px, uint16_t py) {x = px;y = py;};
	uint16_t x,y;
};

class MandelbrotStateEngineEdgeFollow {
public:
	MandelbrotStateEngineEdgeFollow(iterations_t* pp, uint32_t pfullwidth, int32_t pfullheight, uint32_t prender_x, uint32_t prender_y, uint32_t pwidth, uint32_t pheight, double pxs, double pys, double pinc, uint32_t) {
		fullwidth = pfullwidth;
		p = pp + prender_x + fullwidth * prender_y;
		width = pwidth;
		height = pheight;
		dummy_offset = (pfullheight - prender_y - height) * fullwidth - prender_x;
		inc = pinc;
		xs = pxs + prender_x * inc;
		ys = pys - prender_y * inc;
		tail = 15;
		boundaries_done = false;
		wtop = 0;
		wbottom = 0;
		hleft = 1;
		hright = 1;
		todo_list.clear();
	};
	inline void add_to_todo_list(uint32_t x, uint32_t y) {
		todo_list.push_back(Point(x,y));
		p[x + y * fullwidth] = MAX;
	}
	bool get_next_point(double& px, double& py, uint32_t&x, uint32_t& y) {
		if(!boundaries_done) {
			if(wtop<width) {
				setup_point(xs,ys,inc,px,py,x,y,wtop,0);
				wtop++;
				return true;
			} else if(wbottom<width) {
				setup_point(xs,ys,inc,px,py,x,y,wbottom,height - 1);
				wbottom++;
				return true;
			} else if(hleft<height - 1) {
				setup_point(xs,ys,inc,px,py,x,y,0,hleft);
				hleft++;
				return true;
			} else if(hright<height - 1) {
				setup_point(xs,ys,inc,px,py,x,y,width - 1,hright);
				hright++;
				return true;
			} else if(tail != 0) {
				tail--;
				px = py = 0;
				x = 0;
				y = height;
				return true;
			}
			if(todo_list.empty()) {
				// setup initial list of points
				for(uint32_t i = 1;i < width - 1;i++) {
					iterations_t* q = &p[i + fullwidth];
					iterations_t* qq = q - fullwidth;
					if(qq[ - 1] != qq[0] || qq[1] != qq[0]) {
						add_to_todo_list(i,1);
					}
					q = &p[i + fullwidth * (height - 2)];
					qq = q + fullwidth;
					if(qq[ - 1] != qq[0] || qq[1] != qq[0]) {
						add_to_todo_list(i,height - 2);
					}
				}
				for(uint32_t j = 1;j < height - 1;j++) {
					iterations_t* q = &p[j * fullwidth + 1];
					iterations_t* qq = q - 1;
					if(qq[ - fullwidth] != qq[0] || qq[fullwidth] != qq[0]) {
						add_to_todo_list(1,j);
					}
					q = &p[j * fullwidth + (width - 2)];
					qq = q + 1;
					if(qq[ - fullwidth] != qq[0] || qq[fullwidth] != qq[0]) {
						add_to_todo_list(width - 2,j);
					}
				}
				// Deal with the 'Mandelbrot is an island' case
				if(xs < 0.0 && xs + width * inc > 0.0 && ys > 0.0 && ys - height * inc < 0.0) {
					// run a set of TODOs out to the origin from the right
					// Choose x,y corresponding to 0
					uint32_t iy=ys/inc;
					if(iy>0 && iy<height) {
						uint32_t ix=-xs/inc;
						for(uint32_t i = std::max(ix, 1U); i < width - 1; i++) {
							add_to_todo_list(i,iy);
						}
					}
				}
			}
#ifdef EDGE_FOLLOW_QUEUE
			if(todo_index==todo_list.size()) {
				return false;
			}
			Point point = todo_list[todo_index++];
#else
			if(todo_list.empty()) {
				return false;
			}
			Point point = todo_list.back();
			todo_list.pop_back();
#endif
			setup_point(xs,ys,inc,px,py,x,y,point.x,point.y);
			boundaries_done = true;
			return true;
		}
		if(y<height) {
			iterations_t* lastp = p + x + y * fullwidth;
			iterations_t val = lastp[0];
			const int64_t L =  - 1;
			const int64_t R = 1;
			int64_t U =  - fullwidth;
			int64_t D = fullwidth;
			bool diff0 = (lastp[L] != val) && (lastp[L] != 0) && (lastp[L] != MAX);
			bool diff1 = (lastp[R] != val) && (lastp[R] != 0) && (lastp[R] != MAX);
			if(diff0) {
				if(lastp[U + L] == 0) {
					add_to_todo_list(x - 1,y - 1);
				}
				if(lastp[D + L] == 0) {
					add_to_todo_list(x - 1,y + 1);
				}
			}
			if(diff1) {
				if(lastp[U + R] == 0) {
					add_to_todo_list(x + 1,y - 1);
				}
				if(lastp[D + R] == 0) {
					add_to_todo_list(x + 1,y + 1);
				}
			}
			if(diff0 || diff1) {
				if(lastp[U] == 0) {
					add_to_todo_list(x,y - 1);
				}
				if(lastp[D] == 0) {
					add_to_todo_list(x,y + 1);
				}
			}
			diff0 = (lastp[U] != val) && (lastp[U] != 0) && (lastp[U] != MAX);
			diff1 = (lastp[D] != val) && (lastp[D] != 0) && (lastp[D] != MAX);
			if(diff0) {
				if(lastp[U + L] == 0) {
					add_to_todo_list(x - 1,y - 1);
				}
				if(lastp[U + R] == 0) {
					add_to_todo_list(x + 1,y - 1);
				}
			}
			if(diff1) {
				if(lastp[D + L] == 0) {
					add_to_todo_list(x - 1,y + 1);
				}
				if(lastp[D + R] == 0) {
					add_to_todo_list(x + 1,y + 1);
				}
			}
			if(diff0 || diff1) {
				if(lastp[L] == 0) {
					add_to_todo_list(x - 1, y);
				}
				if(lastp[R] == 0) {
					add_to_todo_list(x + 1, y);
				}
			}
		}
#ifdef EDGE_FOLLOW_QUEUE
		while(todo_index<todo_list.size()) {
			Point point = todo_list[todo_index++];
#else
		while(!todo_list.empty()) {
			Point point = todo_list.back();
			todo_list.pop_back();
#endif
			setup_point(xs,ys,inc,px,py,x,y,point.x,point.y);
			tail = 15;
			return true;
		}
		if(tail == 0) {
			return false;
		}
		tail--;
		px = py = 0;
		x = 0;
		y = height;
		return true;
	}
	void cleanup() {
		fill<iterations_t>(p,fullwidth,width,height);
	}
public:
	iterations_t* p;
private:
	int32_t fullwidth;
	uint32_t width;
	uint32_t height;
	int32_t dummy_offset;
	double xs;
	double ys;
	double inc;
	uint32_t tail;
	bool boundaries_done;
	uint32_t wtop;
	uint32_t wbottom;
	uint32_t hleft;
	uint32_t hright;
	std::vector<Point> todo_list;
#ifdef EDGE_FOLLOW_QUEUE
	uint32_t todo_index=0;
#endif
};
