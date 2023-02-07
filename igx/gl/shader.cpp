#include "shader.h"

#include "../../ig/log.h"

#include <stdexcept>
#include <vector>


namespace gl
{
	shader::shader(GLenum type, const void* binary, GLsizei length)
	{
		name_ = glCreateShader(type);

		glShaderBinary(1, &name_, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, binary, length); // vertexData is the SPIR-V file contents
		glSpecializeShader(name_, "main", 0, nullptr, nullptr);

		int res; glGetShaderiv(name_, GL_COMPILE_STATUS, &res);
		std::string log;

		int len; glGetShaderiv(name_, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			std::vector<char> logstr(len);
			int		written = 0;

			glGetShaderInfoLog(name_, len, &written, logstr.data());

			if (written)
				log = logstr.data();

			IGLOG(log);
		}


		if (res == GL_FALSE)
		{
			//std::ostringstream	msg;

			//boost::iostreams::filtering_ostream msg_filter;

			//msg_filter.push(ig::line_counter_output_filter());
			//msg_filter.push(msg);

			////msg_filter << source;
			//msg_filter.flush();

			////msg << source;
			//msg << log;



			throw std::runtime_error(log);
		}


	}

	shader::~shader()
	{
		if (name_)
			glDeleteShader(name_);
	}




}
