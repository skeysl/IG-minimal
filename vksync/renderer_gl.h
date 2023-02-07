#pragma once

#include <igx/gl/program.h>
#include <igx/gl/vertex_array.h>

#include <ig/timer.h>


class renderer_gl
{
public:

	renderer_gl();

	void render(int w, int h, int draw_calls_count);

protected:

	gl::program triangle_program;
	gl::vertex_array vao;

	int time_location;

	ig::high_resolution_timer timer;
};