#pragma once

#ifdef WIN32

#include "../igui_export.h"

#include "window.h"
#include "render_context.h"


namespace igui
{
	namespace win32
	{
		class IGUI_DLL swap_group
		{
		public:

			swap_group() = default;

			swap_group(window & wnd, render_context & rc, int swap_group, int swap_barrier);	
			~swap_group();

			swap_group(const swap_group&) = delete;
			swap_group & operator = (const swap_group&) = delete;

			swap_group(swap_group && other) noexcept;
			swap_group & operator = (swap_group && other) noexcept;

		protected:

			int swap_group_ = 0;
			HDC dc = NULL;
		};
	}
}

#endif