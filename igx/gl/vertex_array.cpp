#include "vertex_array.h"

#include <utility>


namespace gl
{
	vertex_array::vertex_array() : handle_(0)
	{
	}


	vertex_array::vertex_array(int dummy)
	{
		glCreateVertexArrays(1, &handle_);
	}

	vertex_array::vertex_array(vertex_array&& other) noexcept :
		handle_(other.handle_)
	{
		other.handle_ = 0;
	}
		
	vertex_array::~vertex_array()
	{
		if (handle_)
			glDeleteVertexArrays(1, &handle_);
			
	}

	vertex_array& vertex_array::operator = (vertex_array&& other) noexcept
	{
		std::swap(handle_, other.handle_);
		
		return *this;
	}


}

