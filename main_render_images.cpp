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
#include "Mandelbrot_render.h"
#include "MandelbrotStateEngineSimple.h"
#include "MandelbrotStateEngineEdgeFollow.h"

int main() {
	Mandelbrot m5(0,0,0.225,200,200);
	m5.render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	m5.save_to_png("mandelbrota.png",ITERATIONS);
	Mandelbrot m4(TESTD_CX,TESTD_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m4.render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	m4.save_to_png("blacka.png",ITERATIONS);
	Mandelbrot m1(TESTA_CX,TESTA_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m1.render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	m1.save_to_png("test_black_detaila.png",ITERATIONS);
	Mandelbrot m2(TESTB_CX,TESTB_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m2.render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	m2.save_to_png("test_noblack_detaila.png",ITERATIONS);
	Mandelbrot m3(TESTC_CX,TESTC_CY,TEST_ZOOM,WIDTH,HEIGHT);
	m3.render_avx_i4_u_tinc_addint_fast_by4<MandelbrotStateEngineEdgeFollow>(ITERATIONS);
	m3.save_to_png("test_noblack_nodetaila.png",ITERATIONS);
	return 0;

}
