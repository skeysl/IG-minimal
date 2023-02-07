#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>

#ifdef WIN32
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#endif


namespace vk
{

#ifdef WIN32
	using native_memory_handle = HANDLE;
#else
	using native_memory_handle = int;
#endif

	IGX_DLL native_memory_handle get_memory_handle(VkDevice dev, VkDeviceMemory mem);
}
