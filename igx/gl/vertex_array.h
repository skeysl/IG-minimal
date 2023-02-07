#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

namespace gl
{
	class IGX_DLL vertex_array 
	{
	public:

		vertex_array();
		vertex_array(int dummy_to_create);
		~vertex_array();

		vertex_array(vertex_array&&) noexcept;
		vertex_array& operator = (vertex_array&&) noexcept;

		vertex_array(const vertex_array&) = delete;
		vertex_array& operator = (const vertex_array&) = delete;
				
		operator GLuint() const { return handle_; }

	protected:

		GLuint handle_;
	};
}


