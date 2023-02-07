#pragma once

#ifdef WIN32

#include "../igui_export.h"


#include <windows.h>

//#include <GL/glew.h>
//#include <GL/wglew.h>

#include <vulkan/vulkan.h>

#include <vector>


namespace igui
{
	namespace win32
	{
		class IGUI_DLL application
		{
		public:

			application();

			template <class F>
			void for_each_required_instance_extension(F fn) const
			{
				static std::vector<const char*> required_vulkan_extensions = { 
					VK_KHR_SURFACE_EXTENSION_NAME,
					VK_KHR_WIN32_SURFACE_EXTENSION_NAME };

				for (const auto & ext : required_vulkan_extensions)
				{
					fn(ext);
				}
			}

			static void run();
			static bool poll();

			static LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			static void minimize_console();
		};
	}
}
#endif
