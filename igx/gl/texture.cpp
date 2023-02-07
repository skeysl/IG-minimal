#include "texture.h"

#include <cassert>
#include <utility>


namespace gl
{
	texture::texture() :
		target_(0),
		name_(0)
	{
	}

	texture::texture(GLenum target) : target_(target)
	{
		glCreateTextures(target_, 1, &name_);
		assert(name_ != 0);
	}

	texture::~texture()
	{
		if (name_)
			glDeleteTextures(1, &name_);
	}

	texture::texture(texture && other)noexcept :
		target_(other.target_),
		name_(other.name_)
	{
		other.name_ = 0;
	}

	texture & texture::operator = (texture && other) noexcept
	{
		target_ = other.target_;
		name_ = other.name_;
		
		other.name_ = 0;

		return *this;
	}
	

	texture_2d::texture_2d() : texture()
	{
	}

	
	texture_2d::texture_2d(GLsizei levels, GLenum internal_format,
		GLsizei width,
		GLsizei height) : texture(GL_TEXTURE_2D)
	{
		glTextureStorage2D(name_, levels, internal_format, width, height);
	}


	texture_2d::texture_2d(GLsizei levels, GLenum internal_format,
		GLsizei width,
		GLsizei height,
		GLuint memory_object,
		GLuint64 offset) : texture(GL_TEXTURE_2D)
	{
		glTextureStorageMem2DEXT(name_,
			levels,
			internal_format,
			width,
			height,
			memory_object,
			offset);
	}


	texture_2d::texture_2d(texture_2d && other) noexcept : texture(std::move(other))
	{
	}


	texture_2d & texture_2d::operator = (texture_2d && other) noexcept
	{
		texture::operator = (std::move(other));

		return *this;
	}
}
