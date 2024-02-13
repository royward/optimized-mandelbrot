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

#include <cstdint>

class MandelbrotStateEngineSimple {
public:
	MandelbrotStateEngineSimple(iterations_t* pp, uint32_t pfullwidth, int32_t pfullheight, uint32_t prender_x, uint32_t prender_y, uint32_t pwidth, uint32_t pheight, double pxs, double pys, double pinc, uint32_t) {
		fullwidth = pfullwidth;
		p = pp + prender_x + fullwidth * prender_y;
		width = pwidth;
		height = pheight;
		dummy_offset = (pfullheight - prender_y - height) * fullwidth - prender_x;
		inc = pinc;
		xs = pxs + prender_x * inc;
		ys = pys - prender_y * inc;
		w = 0;
		h = 0;
		tail = 11;
	};
	bool get_next_point(double& px, double& py, uint32_t&x, uint32_t& y) {
		if(h == height) {
			if(tail == 0) {
				return false;
			}
			tail--;
			px = py = 0;
			// dummy point
			x = 0;
			y = height;
			return true;
		}
		x = w;
		y = h;
		px = xs + w * inc;
		py = ys - h * inc;
		w++;
		if(w >= width) {
			w = 0;
			h++;
		};
		return true;
	}
	void cleanup() {};
public:
	iterations_t* p;
	uint32_t stored_iterations;
	uint32_t stored_width;
	uint32_t dx[3][4] = {}, dy[3][4] = {};
private:
	int32_t fullwidth;
	uint32_t width;
	uint32_t height;
	int32_t dummy_offset;
	uint32_t w;
	uint32_t h;
	double xs;
	double ys;
	double inc;
	uint32_t tail;
};
