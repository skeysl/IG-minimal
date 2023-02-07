#include "swapchain.h"

#include "../igx_export.h"

#include <stdexcept>


namespace vk
{
	swapchain::swapchain(VkDevice d, const VkSwapchainCreateInfoKHR* create_info) :
		dev(d)
	{
		VkResult err = vkCreateSwapchainKHR(dev, create_info, NULL, &swp);
		if (err != VK_SUCCESS)
		{
			switch (err)
			{
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_OUT_OF_HOST_MEMORY");
				case VK_ERROR_OUT_OF_DEVICE_MEMORY:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_OUT_OF_DEVICE_MEMORY");
				case VK_ERROR_DEVICE_LOST:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_DEVICE_LOST");
				case VK_ERROR_SURFACE_LOST_KHR:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_SURFACE_LOST_KHR");
				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
				case VK_ERROR_INITIALIZATION_FAILED:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_INITIALIZATION_FAILED");
				case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
					throw std::runtime_error("vkCreateSwapchainKHR failed, VK_ERROR_COMPRESSION_EXHAUSTED_EXT");
				default:
					throw std::runtime_error("vkCreateSwapchainKHR failed");
			}
		}
	}


	swapchain::~swapchain()
	{
		if (swp != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(dev, swp, NULL);
	}
}