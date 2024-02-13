// hsv2rgb based on: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

// save_to_png based on: https://gist.github.com/niw/5963798
/*
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng16 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <png.h>
#include <cstdio>
#include <cstdlib>
#include "Mandelbrot.h"

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

png_color hsv2rgb(hsv in) {
	double      hh, p, q, t, ff;
	long        i;
	double rr,gg,bb;
	png_color out;
	if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.red = in.v*255.999;
		out.green = in.v*255.999;
		out.blue = in.v*255.999;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) {
	case 0:
		rr = in.v;
		gg = t;
		bb = p;
		break;
	case 1:
		rr = q;
		gg = in.v;
		bb = p;
		break;
	case 2:
		rr = p;
		gg = in.v;
		bb = t;
		break;
	case 3:
		rr = p;
		gg = q;
		bb = in.v;
		break;
	case 4:
		rr = t;
		gg = p;
		bb = in.v;
		break;
	case 5:
	default:
		rr = in.v;
		gg = p;
		bb = q;
		break;
	}
	out.red=rr*255.999;
	out.green=gg*255.999;
	out.blue=bb*255.999;
	return out;
}

void Mandelbrot::save_to_png(const char* name, uint32_t iterations) {
	png_bytep row_pointers[height];
	uint32_t pixel_size=1;
	FILE *fp = fopen(name, "wb");
	if(!fp) abort();
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_bytep buffer=(png_bytep)png_malloc(png,height*width*pixel_size);
	for (uint32_t i=0; i<height; i++) {
		row_pointers[i]=buffer+i*width*pixel_size;
	}
	png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH*(sizeof(png_color)));
	palette[0].red=0;
	palette[0].green=0;
	palette[0].blue=0;
	palette[1].red=255;
	palette[1].green=255;
	palette[1].blue=255;
	for(uint32_t i=2;i<256;i++) {
		hsv x;
		x.h=360.0*(i-2)/253.0;
		x.s=1.0;
		x.v=1.0;
		palette[i]=hsv2rgb(x);
	}
	if (!png) abort();
	png_infop info = png_create_info_struct(png);
	if (!info) abort();
	if (setjmp(png_jmpbuf(png))) abort();
	png_init_io(png, fp);
	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		width,
		height,
		8,
		PNG_COLOR_TYPE_PALETTE, //PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_set_palette_to_rgb(png);
	png_set_PLTE(png, info, palette, 256);

	png_write_info(png, info);
	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(png, 0, PNG_FILLER_AFTER);
	for(uint32_t i=0; i<height; i++) {
		png_bytep rp=row_pointers[i];
		for(uint32_t j=0; j<width; j++) {
			uint32_t val=p[i*width+j];
			if(val==iterations) {
				rp[j]=0;
			} else if(val==0) {
				rp[j]=1;
			} else {
				rp[j]=(val%0xFE)+2;
			}
		}
	}
	png_write_image(png, row_pointers);
	png_write_end(png, NULL);
	png_free(png,palette);
	png_free(png,buffer);
	fclose(fp);
	png_destroy_write_struct(&png, &info);
}
