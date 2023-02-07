#pragma once

#include "../igx_export.h"

#include <GL/glew.h>


namespace gl
{

	class IGX_DLL shader 
	{
	public:

		shader(GLenum type, const void* binary, GLsizei length);
		~shader();

		operator GLuint() const { return name_; }

	protected:

		GLuint	name_;
	};
}



