#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <stdexcept>


namespace vk
{
	class IGX_DLL swapchain
	{
		VkDevice dev;
		VkSwapchainKHR swp;

	public:

		swapchain() : dev(VK_NULL_HANDLE), swp(VK_NULL_HANDLE) {}

		swapchain(VkDevice d, const VkSwapchainCreateInfoKHR* create_info);
		~swapchain();

		swapchain(swapchain&& other) noexcept :
			dev(other.dev),
			swp(other.swp)
		{
			other.swp = VK_NULL_HANDLE;
		}

		swapchain& operator = (swapchain&& other) noexcept
		{
			std::swap(dev, other.dev);
			std::swap(swp, other.swp);

			return *this;
		}

		swapchain(const swapchain&) = delete;
		swapchain & operator = (const swapchain&) = delete;

		operator VkSwapchainKHR() const { return swp; }
	};

}
