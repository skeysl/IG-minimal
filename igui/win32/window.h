#pragma once

#ifdef WIN32

#include "../igui_export.h"

#include <windows.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <utility>
#include <optional>


namespace igui
{
	namespace win32
	{
		class IGUI_DLL window
		{
		public:

			window() = default;

			window(int x, int y, int width, int height, const char * title,
				std::optional<int> monitor = {});// bool debug = false, int swap_group = -1, int swap_barrier = -1)
			window(int width, int height, const char * title,
				std::optional<int> monitor = {});
			

			window(int x, int y, int width, int height, const char * title,
				std::initializer_list<int> pf_attribs,
				std::initializer_list<int> context_attribs = { 0 },
				std::optional<int> monitor = {}, std::optional<int> affinity = {});// bool debug = false, int swap_group = -1, int swap_barrier = -1)


			window(int width, int height, const char * title,
				std::initializer_list<int> pf_attribs,
				std::initializer_list<int> context_attribs = { 0 },
				std::optional<int> monitor = {}, std::optional<int> affinity = {});  //, bool debug = false, int swap_group = -1, int swap_barrier = -1) : 

			window(const window&) = delete;
			window & operator = (const window&) = delete;

			window(window && other) noexcept;
			window & operator = (window && other) noexcept;

			~window();

			operator const HWND & () const { return wnd; }


			void enum_devices();

			void swap_buffers();

			std::pair<int, int> size() const;

			HGLRC render_context() const { return rc; }
			HDC device_context() const { return dc; }
		
			HWND wnd = NULL;
			HDC dc = NULL;
			HGLRC rc = NULL;
		};
	}
}
#endif
