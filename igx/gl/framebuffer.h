#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

#include <initializer_list>


namespace gl
{
	class IGX_DLL framebuffer
	{
	public:
	
		struct framebufer_texture_desc
		{
			GLenum attachment;
			GLuint tex;
			int mipmap;
			int layer;
			
			GLint base_view_index;
			GLsizei num_views;

			framebufer_texture_desc(GLenum attach, GLuint t, int mip = 0, int l = -1, GLint base_view = 0, GLsizei views = 0) : attachment(attach), tex(t), mipmap(mip), layer(-1),
				base_view_index(base_view),
				num_views(views)
			{
			}
		};

		framebuffer();
		framebuffer(std::initializer_list<framebufer_texture_desc> attachments, std::initializer_list<GLenum> draw_buffers = {});
		
		framebuffer(framebuffer&&) noexcept;
		framebuffer& operator = (framebuffer&&) noexcept;
				
		~framebuffer();
				
		
		framebuffer(const framebuffer&) = delete;
		framebuffer & operator = (const framebuffer &) = delete;
		
		framebuffer & framebuffer_texture(GLenum attachment, GLuint tex, int mipmap = 0);
		framebuffer & framebuffer_texture_multiview(GLenum attachment, GLuint tex, int level, GLint base_view_index, GLsizei num_views);
		framebuffer & framebuffer_texture_layer(GLenum attachment, GLuint tex, int mipmap, int layer);
		framebuffer & framebuffer_renderbuffer(GLenum attachment, GLuint rb);
		framebuffer & draw_buffer(GLenum buffer);
		framebuffer & draw_buffers(std::initializer_list<GLenum> buffers);
		framebuffer & status();

		operator GLuint() const { return handle_; }

	protected:

		GLuint handle_;

	};
}

//IG_TEMPLATE_DLL template class IGX_DLL std::shared_ptr<igx::gl::framebuffer>;
