#include "device.h"

#include <stdexcept>


namespace vk
{
	device::device() : dev(VK_NULL_HANDLE) 
	{
	}

	device::device(VkPhysicalDevice gpu, const VkDeviceCreateInfo* create_info)
	{
		VkResult res = vkCreateDevice(gpu, create_info, NULL, &dev);
		if (res != VK_SUCCESS)
		{
			throw std::runtime_error("vkCreateDevice failed.");
		}
	}

	device::~device()
	{
		if (dev)
			vkDestroyDevice(dev, NULL);
	}

	device::device(device&& other) noexcept : dev(other.dev)
	{
		other.dev = VK_NULL_HANDLE;
	}

	device& device::operator = (device&& other) noexcept
	{
		std::swap(dev, other.dev);
		return *this;
	}
}
