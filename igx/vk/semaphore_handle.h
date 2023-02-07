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
	using native_semaphore_handle = HANDLE;
#else
	using native_semaphore_handle = int;
#endif

	IGX_DLL native_semaphore_handle get_semaphore_handle(VkDevice dev, VkSemaphore s);

	IGX_DLL void import_semaphore_handle(VkDevice dev, VkSemaphore s, native_semaphore_handle h, 
#ifdef WIN32
		VkExternalSemaphoreHandleTypeFlagBits handle_type = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT
#else
		VkExternalSemaphoreHandleTypeFlagBits handle_type = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT
#endif
	);
}
