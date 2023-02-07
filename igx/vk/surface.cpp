#include "surface.h"

#include <stdexcept>

namespace vk
{

#if ((defined WIN32) && (defined VK_USE_PLATFORM_WIN32_KHR))
	surface::surface(VkInstance instance, HWND wnd, HINSTANCE hinst) : inst(instance)
	{
		auto fnCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(inst, "vkCreateWin32SurfaceKHR");
		if (!fnCreateWin32SurfaceKHR)
			throw std::runtime_error("vkCreateWin32SurfaceKHR not found");

		VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
		createInfo.hwnd = wnd;
		createInfo.hinstance = hinst;
		
		VkResult res = fnCreateWin32SurfaceKHR(inst, &createInfo, nullptr, &s);
		if (res != VK_SUCCESS)
			throw std::runtime_error("vkCreateWin32SurfaceKHR failed");
	}
#elif (defined VK_USE_PLATFORM_XLIB_KHR)
	surface::surface(VkInstance instance, Display* dpy, Window window) : inst(instance)
	{
		VkXlibSurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR };
		createInfo.dpy = dpy;
		createInfo.window = window;

		auto CreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)vkGetInstanceProcAddr(inst, "vkCreateXlibSurfaceKHR");
		if (!CreateXlibSurfaceKHR)
			throw std::runtime_error("vkCreateXlibSurfaceKHR not found");

		VkResult res = CreateXlibSurfaceKHR(inst, &createInfo, nullptr, &s);
		if (res != VK_SUCCESS)
			throw std::runtime_error("CreateXlibSurfaceKHR failed");
	}
#elif (defined VK_USE_PLATFORM_XCB_KHR)
	surface::surface(VkInstance instance, xcb_connection_t* connection, xcb_window_t window) : inst(instance)
	{
		VkXcbSurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR; }
		createInfo.connection = connection;
		createInfo.window = window;

		auto CreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)vkGetInstanceProcAddr(inst, "vkCreateXcbSurfaceKHR");
		if (!CreateXcbSurfaceKHR)
			throw std::runtime_error("vkCreateXcbSurfaceKHR not found");

		VkResult res = CreateXcbSurfaceKHR(inst, &createInfo, nullptr, &s);
		if (res != VK_SUCCESS)
			throw std::runtime_error("CreateXcbSurfaceKHR failed");
	}
#elif (defined VK_USE_PLATFORM_WAYLAND_KHR)
	surface::surface(VkInstance instance, struct wl_display* display, struct wl_surface* surface) : inst(instance)
	{
		VkWaylandSurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR; }
		createInfo.display = display;
		createInfo.surface = surface;

		auto CreateWaylandSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)vkGetInstanceProcAddr(inst, "vkCreateWaylandSurfaceKHR");
		if (!CreateWaylandSurfaceKHR)
			throw std::runtime_error("vkCreateWaylandSurfaceKHR not found");

		VkResult res = CreateWaylandSurfaceKHR(inst, &createInfo, nullptr, &s);
		if (res != VK_SUCCESS)
			throw std::runtime_error("CreateWaylandSurfaceKHR failed");
	}
#endif

	surface::~surface()
	{
		if (s)
		{
			auto DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(inst, "vkDestroySurfaceKHR");

			if (DestroySurfaceKHR)
				DestroySurfaceKHR(inst, s, NULL);
		}
	}
}