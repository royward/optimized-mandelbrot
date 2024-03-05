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

#include "Mandelbrot.h"
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include "MandelbrotStateEngineEdgeFollow.h"

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

struct DisplayParams {
	uint32_t x,y,width,height;
};

void task(Mandelbrot* m, std::mutex* lock, std::vector<DisplayParams>* job_list, uint32_t iterations) {
	while(true) {
		lock->lock();
		if(job_list->empty()) {
			lock->unlock();
			return;
		}
		DisplayParams dp=job_list->back();
		job_list->pop_back();
		lock->unlock();
		m->render_avx_sheeprace4_u_tinc_addint_fast_sr_by4<MandelbrotStateEngineEdgeFollow>(iterations,dp.x,dp.y,dp.width,dp.height);
	}
}

void Mandelbrot::render_multithreaded(uint32_t iterations, uint32_t thread_count, uint32_t divx, uint32_t divy) {
	std::vector<DisplayParams> job_list;
	std::unique_ptr<std::thread[]> threads = std::make_unique<std::thread[]>(thread_count);
	std::mutex lock;
	if(divy==0) {
		divy=divx;
	}
	for(uint32_t j=0;j<divy;j++) {
		for(uint32_t i=0;i<divx;i++) {
			DisplayParams dp;
			dp.x=(width*i)/divx;
			dp.y=(height*j)/divy;
			dp.width=(width*(i+1))/divx-(width*i)/divx;
			dp.height=(height*(j+1))/divy-(height*j)/divy;
			job_list.push_back(dp);
		}
	}
	for(uint32_t i=0;i<thread_count;i++) {
		threads[i]=std::thread(&task,this,&lock,&job_list,iterations);
	}
	for(uint32_t i=0;i<thread_count;i++) {
		threads[i].join();
	}
}
