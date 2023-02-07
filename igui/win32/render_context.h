#pragma once

#ifdef _WIN32

#include "../igui_export.h"

//#include "../ig/log.h"

//#include <exception>
//#include <iostream>
//#include <vector>

#include <windows.h>
//#include <tchar.h>

#include <GL/glew.h>
//#include <GL/wglew.h>




namespace igui
{
	namespace win32
	{
		class window;

		class IGUI_DLL render_context
		{
		public:

			render_context() = default;

			render_context(window & w);
			~render_context();

			render_context(const render_context&) = delete;
			render_context & operator = (const render_context&) = delete;

			render_context(window && other) noexcept;
			render_context & operator = (render_context && other) noexcept;

		protected:

			HDC dc = NULL;
			HGLRC rc = NULL;
		};
	}
}
#endif
