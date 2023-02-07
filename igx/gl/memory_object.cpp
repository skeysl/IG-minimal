#include "memory_object.h"

#include <utility>

namespace gl
{
	memory_object::memory_object() : m(0)
	{
	}

#ifdef WIN32
	memory_object::memory_object(GLuint64 size, void *handle, GLenum handleType) : m(0)
	{
		glCreateMemoryObjectsEXT(1, &m);
		glImportMemoryWin32HandleEXT(m,
			size,
			handleType,
			handle);
	}

#else

	memory_object::memory_object(GLuint64 size, int fd, GLenum handleType) : m(0)
	{
		glCreateMemoryObjectsEXT(1, &m);
		glImportMemoryFdEXT(m, size, handleType, fd);
	}

#endif

	memory_object::memory_object(memory_object && other) noexcept : 
		m(other.m)
	{
		other.m = 0;
	}

	memory_object::~memory_object()
	{
		if (m)
			glDeleteMemoryObjectsEXT(1, &m);
	}

	memory_object & memory_object::operator = (memory_object && other) noexcept
	{
		std::swap(m, other.m); return *this;
	}

	void memory_object::parameter(GLenum pname, const int * params)
	{
		glMemoryObjectParameterivEXT(m, pname, params);
	}
}
