#include "device_memory_handle.h"

#include <stdexcept>


namespace vk
{

	native_memory_handle get_memory_handle(VkDevice dev, VkDeviceMemory mem)
	{
#ifdef WIN32
		PFN_vkGetMemoryWin32HandleKHR fnGetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)vkGetDeviceProcAddr(dev, "vkGetMemoryWin32HandleKHR");

		VkMemoryGetWin32HandleInfoKHR get_info{ VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR };
		get_info.memory = mem;
		get_info.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;

		HANDLE h = INVALID_HANDLE_VALUE;
		VkResult res = fnGetMemoryWin32HandleKHR(dev, &get_info, &h);
		if (res != VK_SUCCESS)
			throw std::runtime_error("vkGetMemoryWin32HandleKHR failed");

		return h;
#else

		auto fnGetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)vkGetDeviceProcAddr(dev, "vkGetMemoryFdKHR");

		VkMemoryGetFdInfoKHR get_info{ VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR };
		get_info.memory = mem;
		get_info.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;

		int fd;
		VkResult res = fnGetMemoryFdKHR(dev, &get_info, &fd);
		if (res != VK_SUCCESS)
			throw std::runtime_error("vkGetMemoryFdKHR failed");

		return fd;
#endif
	}
}
