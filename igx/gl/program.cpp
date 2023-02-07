#include "../../ig/log.h"
#include "program.h"
#include "shader.h"


#include <vector>
#include <string>

namespace gl
{
	program::program() :
		handle_(0)
	{
	}

	program::program(program && other) noexcept :
		handle_(other.handle_)
	{
		other.handle_ = 0;
	}
		

	program::program(const shader & s1) : handle_(glCreateProgram())
	{
		glAttachShader(handle_, s1);
		link();
	}


	program::program(const shader & s1, const shader & s2) : handle_(glCreateProgram())
	{
		glAttachShader(handle_, s1);
		glAttachShader(handle_, s2);
		link();
	}


	program::~program()
	{
		if (handle_)
			glDeleteProgram(handle_);
	}


	program & program::operator = (program && other) noexcept
	{
		std::swap(handle_, other.handle_);

		return *this;
	}


	void program::link()
	{

		/*if (!transform_feedback_varyings.empty())
		{
		std::vector<const char *>	vars(transform_feedback_varyings.size());
		for (size_t i = 0; i < transform_feedback_varyings.size(); i++)
		vars[i] = transform_feedback_varyings[i].c_str();

		glTransformFeedbackVaryings(handle_, vars.size(), &vars[0], transform_feedback_mode);
		}

		for (std::map<std::string, int>::const_iterator it = attribs_binding.begin(); it != attribs_binding.end(); ++it)
		{
		glBindAttribLocation(handle_, it->second, it->first.c_str());
		}

		for (std::map<std::string, int>::const_iterator it = outputs_binding.begin(); it != outputs_binding.end(); ++it)
		{
		glBindFragDataLocation(handle_, it->second, it->first.c_str());
		}*/

		//assert(glGetError() == GL_NO_ERROR);

		glLinkProgram(handle_);

		
			std::string log;

			int len; glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				std::vector<char> logstr(len);
				int		written = 0;

				glGetProgramInfoLog(handle_, len, &written, logstr.data());

				if (written)
					log = logstr.data();

				IGLOG(log);
			}

			int res; glGetProgramiv(handle_, GL_LINK_STATUS, &res);
			if (res == GL_FALSE)
			{

			/*std::ostringstream	msg;

			BOOST_FOREACH(const shader_ptr & s, p->shaders)
			{
			boost::iostreams::filtering_ostream msg_filter;

			msg_filter.push(line_counter_output_filter());
			msg_filter.push(msg);

			msg_filter << s->get_source();
			msg_filter.flush();
			}

			msg << log;*/

			throw std::runtime_error(log);
		}

		/*int active_uniforms; glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &active_uniforms);
		for (GLint i = 0; i < active_uniforms; ++i)
		{
		GLsizei name_length; glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_length);
		GLint size;
		GLenum type;
		GLchar * name = new GLchar[name_length];

		glGetActiveUniform(handle_, i, name_length, nullptr, &size, &type, name);

		std::string name_str(name);
		uniforms[name_str] = glGetUniformLocation(handle_, name);

		delete[] name;
		}*/

		/*int active_uniform_blocks; glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_BLOCKS, &active_uniform_blocks);
		for (int i = 0; i < active_uniform_blocks; i++)
		{
		GLsizei name_length; glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &name_length);
		GLchar * name = new GLchar[name_length];

		glGetActiveUniformBlockName(handle_, i, name_length, nullptr, name);

		std::string name_str(name);
		uniform_blocks[name_str] = i;

		delete[] name;
		}*/

		/*for (std::map<std::string, int>::const_iterator it = uniform_blocks_binding.begin(); it != uniform_blocks_binding.end(); ++it)
		{
		int block_idx = glGetUniformBlockIndex(handle_, it->first.c_str());
		if (block_idx != -1)
		glUniformBlockBinding(handle_, block_idx, it->second);
		}

		*/

		//BOOST_FOREACH(const uniform_ptr & u, const_uniforms)
		//	u->push(this);

		/*GLuint active_attributes = geti(GL_ACTIVE_ATTRIBUTES);
		for (GLuint i = 0; i < active_attributes; i++)
		{
		GLsizei name_length = geti(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
		GLint size;
		GLenum type;
		GLchar * name = new GLchar [name_length];

		glGetActiveAttrib(*handle_, i, name_length, nullptr, &size, &type, name);

		attrib_locations[std::string(name)] = glGetAttribLocation(*handle_, name);
		}*/

		//glBindFragDataLocation(handle_, id, var.c_str());
	}


	int program::uniform_location(const char * name)
	{
		return glGetUniformLocation(handle_, name);
	}

	program & program::uniform(GLint location, float x)
	{
		glProgramUniform1f(handle_, location, x); return *this;
	}
}
