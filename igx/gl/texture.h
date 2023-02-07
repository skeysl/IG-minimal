#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

namespace gl
{

	class IGX_DLL texture 
	{
	protected:

		GLenum target_;
		GLuint name_;

	public:

		texture();
		texture(GLenum target);
		~texture();

		texture(texture &&) noexcept;
		texture & operator = (texture &&) noexcept;

		texture(const texture &) = delete;
		texture & operator = (const texture &) = delete;
				
		operator GLuint() const { return name_; }
	};
	

	class IGX_DLL texture_2d : public texture
	{
	public:

		texture_2d();
		/*texture_2d(GLsizei levels, GLenum internal_format,
			GLsizei width,
			GLsizei height);*/
		texture_2d(GLsizei levels, GLenum internal_format,
			GLsizei width,
			GLsizei height);
		texture_2d(GLsizei levels, GLenum internal_format,
			GLsizei width,
			GLsizei height,
			GLuint memory_object, GLuint64 offset = 0);

		texture_2d(texture_2d &&) noexcept;

		texture_2d(const texture_2d &) = delete;

		texture_2d & operator = (texture_2d &&) noexcept;
		texture_2d & operator = (const texture_2d &) = delete;
	};
}
