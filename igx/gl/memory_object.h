#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

namespace gl
{
	class IGX_DLL memory_object
	{
	public:

		memory_object();

#ifdef WIN32
		memory_object(GLuint64 size, void *handle,
			GLenum handleType = GL_HANDLE_TYPE_OPAQUE_WIN32_EXT);
#else
		memory_object(GLuint64 size, int fd,
			GLenum handleType = GL_HANDLE_TYPE_OPAQUE_FD_EXT);
#endif
		
		memory_object(memory_object &&) noexcept;
		memory_object(const memory_object &) = delete;
		~memory_object();
		
		memory_object & operator = (memory_object &&) noexcept;
		memory_object & operator = (const memory_object &) = delete;
		
		void parameter(GLenum pname, const int * params);
		
		operator GLuint() const { return m; }

	protected:

		GLuint m;
	};
}
