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
