#pragma once

#include <igx/vk/instance.h>
#include <igx/vk/debug_utils_messenger.h>

#include <vector>


class vulkan_instance
{
public:

	vulkan_instance(
		const std::vector<const char*>& instance_extensions, bool debug);

	VkInstance instance() const { return inst; }
	VkPhysicalDevice physical_device(unsigned index) const 
	{
		return physical_devices.at(index);
	}

protected:

	vk::instance inst;
	vk::debug_utils_messenger debug_messenger;

	std::vector<VkPhysicalDevice> physical_devices;
};
