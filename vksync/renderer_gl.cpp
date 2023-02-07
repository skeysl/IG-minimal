#include "renderer_gl.h"

#include <igx/gl/shader.h>

const uint32_t triangle_gl_vert_sprv[] = 
{
#include "triangle_gl.vert.sprv"
};

const uint32_t triangle_gl_frag_sprv[] = 
{
#include "triangle_gl.frag.sprv"
};


renderer_gl::renderer_gl() :
	vao(0)
{
	triangle_program = gl::program(
		gl::shader(GL_VERTEX_SHADER, (const uint32_t *)triangle_gl_vert_sprv, sizeof(triangle_gl_vert_sprv)),
		gl::shader(GL_FRAGMENT_SHADER, (const uint32_t *)triangle_gl_frag_sprv, sizeof(triangle_gl_frag_sprv)));

	time_location = triangle_program.uniform_location("time");
}


void renderer_gl::render(int w, int h, int draw_calls_count)
{
	

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, w, h);

	glBindVertexArray(vao);

	glUseProgram(triangle_program);
	float t = timer.elapsed();
	triangle_program.uniform(time_location, t);
	for (int i = 0; i < draw_calls_count; i++)
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);

	glBindVertexArray(0);
}