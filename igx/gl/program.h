#pragma once

#include "../igx_export.h"

#include <GL/glew.h>

namespace gl
{
	class IGX_DLL shader;
	
	class IGX_DLL program 
	{
	public:

		program();
		program(const shader & s1);
		program(const shader & s1, const shader & s2);
		
		program(program &&) noexcept;
		program(const program &) = delete;

		~program();

		program & operator = (program &&) noexcept;
		program & operator = (const program &) = delete;

		int uniform_location(const char * name);

		program& uniform(GLint location, float x);

		program & uniform(const char * u, float x) { return uniform(uniform_location(u), x); }
						
		operator GLuint() const { return handle_; }

	protected:

		void link();
		
		GLuint handle_;
	};
}

//IG_TEMPLATE_DLL template class IGX_DLL std::shared_ptr<igx::gl::program>;
