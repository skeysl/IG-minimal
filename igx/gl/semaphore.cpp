#include "semaphore.h"

#include <utility>
#include <stdexcept>


namespace gl
{
	semaphore::semaphore() : s(0)
	{
	}

#ifdef WIN32
	semaphore::semaphore(void * h, GLenum handle_type) : s(0)
	{
		glGenSemaphoresEXT(1, &s);
		if (!s)
			throw std::runtime_error("glGenSemaphoresEXT failed!");

		glImportSemaphoreWin32HandleEXT(s, handle_type, h);
	}
#else
	semaphore::semaphore(int fd, GLenum handle_type) : s(0)
	{
		glGenSemaphoresEXT(1, &s);
		glImportSemaphoreFdEXT(s, handle_type, fd);
	}
#endif

	semaphore::semaphore(semaphore && other) noexcept :
		s(other.s)
	{
		other.s = 0;
	}

	semaphore::~semaphore()
	{
		if (s)
			glDeleteSemaphoresEXT(1, &s);
	}

	semaphore & semaphore::operator = (semaphore && other) noexcept
	{
		std::swap(s, other.s); return *this;
	}

	void semaphore::parameter(GLenum pname, const GLuint64  * params)
	{
		glSemaphoreParameterui64vEXT(s, pname, params);
	}

	void semaphore::wait_semaphore(GLuint numBufferBarriers, const GLuint *buffers,
		GLuint numTextureBarriers, const GLuint *textures,
		const GLenum *srcLayouts)
	{
		glWaitSemaphoreEXT(s,
			numBufferBarriers, buffers,
			numTextureBarriers, textures,
			srcLayouts);
	}

	void semaphore::wait_texture_semaphore(GLuint tex, GLenum src_layout)
	{
		wait_semaphore(0, nullptr, 1, &tex, &src_layout);
	}

	void semaphore::signal_semaphore(GLuint numBufferBarriers, const GLuint *buffers,
		GLuint numTextureBarriers, const GLuint *textures,
		const GLenum *dstLayouts)
	{
		glSignalSemaphoreEXT(s,
			numBufferBarriers, buffers,
			numTextureBarriers, textures,
			dstLayouts);
	}

	void semaphore::signal_texture_semaphore(GLuint tex, GLenum dst_layout)
	{
		signal_semaphore(0, nullptr, 1, &tex, &dst_layout);
	}
}
