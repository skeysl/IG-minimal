#include "framebuffer.h"
#include <stdexcept>

namespace gl
{
	framebuffer::framebuffer() : handle_(0)
	{
	}
	
	framebuffer::framebuffer(std::initializer_list<framebufer_texture_desc> attachments, std::initializer_list<GLenum> draw_buf)
	{
		glCreateFramebuffers(1, &handle_);

		for (const auto & a : attachments)
			if (a.num_views != 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, handle_);
				glFramebufferTextureMultiviewOVR(GL_FRAMEBUFFER, a.attachment, a.tex, a.mipmap, a.base_view_index, a.num_views);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else
				if (a.layer != -1)
					framebuffer_texture_layer(a.attachment, a.tex, a.mipmap, a.layer);
				else
					framebuffer_texture(a.attachment, a.tex, a.mipmap);

		if (draw_buf.size() != 0)
			draw_buffers(draw_buf);

		status();
	}

	framebuffer::framebuffer(framebuffer && other) noexcept : handle_(other.handle_)
	{
		other.handle_ = 0;
	}


	framebuffer::~framebuffer()
	{
		if (handle_)
			glDeleteFramebuffers(1, &handle_);
	}

	framebuffer & framebuffer::operator = (framebuffer && other) noexcept
	{
		std::swap(handle_, other.handle_); return *this;
	}

	
	framebuffer & framebuffer::framebuffer_texture(GLenum attachment, GLuint tex, int mipmap)
	{
		glNamedFramebufferTexture(handle_, attachment, tex, mipmap);

		return *this;
	}

	framebuffer & framebuffer::framebuffer_texture_multiview(GLenum attachment, GLuint tex, int level, GLint base_view_index, GLsizei num_views)
	{
		//glNamedFramebufferTextureMultiviewOVR(handle_, attachment, tex, level, base_view_index, num_views);
		glBindFramebuffer(GL_FRAMEBUFFER, handle_);
		glFramebufferTextureMultiviewOVR(GL_FRAMEBUFFER, attachment, tex, level, base_view_index, num_views);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return *this;
	}


	framebuffer & framebuffer::framebuffer_texture_layer(GLenum attachment, GLuint tex, int mipmap, int layer)
	{
		glNamedFramebufferTextureLayer(handle_, attachment, tex, mipmap, layer);

		return *this;
	}


	framebuffer & framebuffer::framebuffer_renderbuffer(GLenum attachment, GLuint rb)
	{
		glNamedFramebufferRenderbuffer(handle_, attachment, GL_RENDERBUFFER, rb);
		return *this;
	}	


	framebuffer & framebuffer::draw_buffer(GLenum buffer)
	{
		glNamedFramebufferDrawBuffer(handle_, buffer);

		return *this;
	}


	framebuffer & framebuffer::draw_buffers(std::initializer_list<GLenum> buffers)
	{

		glNamedFramebufferDrawBuffers(handle_, (GLsizei) buffers.size(), buffers.begin());

		return *this;
	}


	framebuffer & framebuffer::status()
	{
		GLenum status = glCheckNamedFramebufferStatus(handle_, GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (status)
			{
				case GL_FRAMEBUFFER_UNDEFINED: throw std::runtime_error("GL_FRAMEBUFFER_UNDEFINED");
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
				case GL_FRAMEBUFFER_UNSUPPORTED: throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED");
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
				default:
					throw std::runtime_error("GL_FRAMEBUFFER not complete");
			}
		}

		return *this;
	}
}

