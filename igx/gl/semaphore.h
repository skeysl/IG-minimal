#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

namespace gl
{
	class IGX_DLL semaphore
	{
	public:

		semaphore();
#ifdef WIN32
		semaphore(void * h, GLenum handle_type = GL_HANDLE_TYPE_OPAQUE_WIN32_EXT);
#else
		semaphore(int fd, GLenum handle_type = GL_HANDLE_TYPE_OPAQUE_FD_EXT);
#endif
		semaphore(semaphore &&) noexcept;
		semaphore(const semaphore &) = delete;
		~semaphore();
		
		semaphore & operator = (semaphore &&) noexcept;
		semaphore & operator = (const semaphore &) = delete;
		
		void parameter(GLenum pname, const GLuint64  * params);

		void wait_semaphore(
			GLuint numBufferBarriers, const GLuint *buffers,
			GLuint numTextureBarriers, const GLuint *textures,
			const GLenum *srcLayouts);

		void wait_texture_semaphore(GLuint tex, GLenum src_layout);

		void signal_semaphore(
			GLuint numBufferBarriers, const GLuint *buffers,
			GLuint numTextureBarriers, const GLuint *textures,
			const GLenum *dstLayouts);

		void signal_texture_semaphore(GLuint tex, GLenum dst_layout);

		operator GLuint() const { return s; }

	protected:

		GLuint s;
	};

}
