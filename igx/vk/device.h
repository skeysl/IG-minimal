#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	
	class IGX_DLL device
	{
		VkDevice dev;

	public:

		device();

		device(VkPhysicalDevice gpu, const VkDeviceCreateInfo* create_info);
		~device();

		device(device&& other) noexcept;

		device& operator = (device&& other) noexcept;

		device(const device &) = delete;
		device & operator = (const device &) = delete;


		operator VkDevice() const { return dev; }
	};
}
