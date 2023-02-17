#ifdef WIN32
#include "render_context.h"

#include "window.h"

#include <stdexcept>


namespace igui::win32
{
	render_context::render_context(window& w) :
		dc(w.dc),
		rc(w.rc)
	{
		if (!wglMakeCurrent(dc, rc))
		{
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

			LocalFree(lpMsgBuf);

			throw std::runtime_error("wglMakeCurrent failed!");
		}

		if (glewInit() != GLEW_OK)
			throw std::runtime_error("glewInit failed!");
	}


	render_context::~render_context()
	{
		if (dc && rc)
			wglMakeCurrent(NULL, NULL);
	}


	render_context::render_context(window&& other) noexcept :
		dc(other.dc),
		rc(other.rc)
	{
		other.dc = NULL;
		other.rc = NULL;
	}


	render_context& render_context ::operator = (render_context&& other) noexcept
	{
		std::swap(dc, other.dc);
		std::swap(rc, other.rc);

		return *this;
	}
}

#endif
