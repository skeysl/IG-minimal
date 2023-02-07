#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>

#ifdef VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#endif

namespace vk
{

	class IGX_DLL surface
	{
		VkSurfaceKHR s;
		VkInstance inst;

	public:

#if ((defined WIN32) && (defined VK_USE_PLATFORM_WIN32_KHR))
		surface(VkInstance instance, HWND wnd, HINSTANCE hinst = GetModuleHandle(nullptr));
#elif (defined VK_USE_PLATFORM_XLIB_KHR)
		surface(VkInstance instance, Display* dpy, Window window);
#elif (defined VK_USE_PLATFORM_XCB_KHR)
		surface(VkInstance instance, xcb_connection_t* connection, xcb_window_t window);
#elif (defined VK_USE_PLATFORM_WAYLAND_KHR)
		surface(VkInstance instance, struct wl_display* display, struct wl_surface* surface);
#endif
		~surface();

		operator const VkSurfaceKHR & () const { return s; }
	};
}
