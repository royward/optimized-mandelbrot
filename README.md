# optimized-mandelbrot
A set of optimizations of a Mandelbrot Set for x86-64 generator that gives ~8x speed improvement over the basic loop. This is designed to go with https://orange-kiwi.com/optimization-example-mandelbrot-part-1/

This currently only builds under Linux. but would be easy to port to other operating systems. To build, use the command:

<pre>make</pre>

There are two build targets:

* mandelbrot_speed_tests runs the speed tests.

* mandelbrot_render_images generates the images used by the web page.

## List of common files

### Mandelbrot.h, Mandelbrot.cpp 

These are the files setting up the class

### Mandelbrot_fastest.cpp

This is the fastest version of the Mandelbrot Set generator (so far)

### Mandelbrot_render.h

These are various container rendering methods.

### MandelbrotStateEngineSimple.h

A state engine to go with Mandelbrot_render.h

## List of files used by the image render example

### main_render_images.cpp

The main for rendering images.

### Mandelbrot_create_png.cpp

Some code for creating a png, adapted from various sources. Some of this uses the X11 license.

## List of files used by the speed test

### main_speed_tests.cpp

The main for the speed tests.

### speed_tests_fast.cpp

Portions of the speed test need to be build with -ffast-math. They have been moved from main_speed_tests.cpp to here.

## List of files used by the speed test that aren't are made obsolete by faster methods

### Mandelbrot_point_simple.cpp

This is the point generation function built without -ffast-math

### Mandelbrot_functions_fast.cpp

These are various point generation functions built with -ffast-math.

### Mandelbrot_render_simple.h

Outer loops for Mandelbrot_point_simple.cpp and Mandelbrot_functions_fast.cpp
