#include "semaphore_handle.h"

#include <stdexcept>
#include <cassert>

namespace vk
{
	native_semaphore_handle get_semaphore_handle(VkDevice dev, VkSemaphore s)
	{
#ifdef WIN32
		PFN_vkGetSemaphoreWin32HandleKHR fnGetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR)vkGetDeviceProcAddr(dev, "vkGetSemaphoreWin32HandleKHR");
		if (!fnGetSemaphoreWin32HandleKHR)
			throw std::runtime_error("vkGetSemaphoreWin32HandleKHR not present");

		VkSemaphoreGetWin32HandleInfoKHR get_info{ VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR };
		get_info.semaphore = s;
		get_info.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT;

		HANDLE h = INVALID_HANDLE_VALUE;
		VkResult res = fnGetSemaphoreWin32HandleKHR(dev, &get_info, &h);
		if (res != VK_SUCCESS)
			throw std::runtime_error("fnGetSemaphoreWin32HandleKHR failed");

		return h;
#else
		auto fnGetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)vkGetDeviceProcAddr(dev, "vkGetSemaphoreFdKHR");
		if (!fnGetSemaphoreFdKHR)
			throw std::runtime_error("vkGetSemaphoreFdKHR not present");

		VkSemaphoreGetFdInfoKHR get_info{ VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR };
		get_info.semaphore = s;
		get_info.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT;

		int fd;
		VkResult res = fnGetSemaphoreFdKHR(dev, &get_info, &fd);
		if (res != VK_SUCCESS)
			throw std::runtime_error("fnGetSemaphoreFdKHR failed");

		return fd;
#endif
	}


	void import_semaphore_handle(VkDevice dev, VkSemaphore s, native_semaphore_handle h, VkExternalSemaphoreHandleTypeFlagBits handle_type)
	{
#ifdef WIN32
		PFN_vkImportSemaphoreWin32HandleKHR fnImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR)vkGetDeviceProcAddr(dev, "vkImportSemaphoreWin32HandleKHR");
		if (!fnImportSemaphoreWin32HandleKHR)
			throw std::runtime_error("vkImportSemaphoreWin32HandleKHR not present");

		VkImportSemaphoreWin32HandleInfoKHR import_info{ VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR };
		import_info.semaphore = s;
		import_info.handleType = handle_type;//VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT;
											//VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT
		import_info.handle = h;

		VkResult res = fnImportSemaphoreWin32HandleKHR(dev, &import_info);
		if (res != VK_SUCCESS)
			throw std::runtime_error("vkImportSemaphoreWin32HandleKHR failed");
#else
		// todo:
		assert(0);
#endif
	}

}
