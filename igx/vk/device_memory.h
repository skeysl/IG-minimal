#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{

	class IGX_DLL device_memory
	{
		VkDeviceMemory mem = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		device_memory() = default;

		device_memory(VkDevice d, const VkMemoryAllocateInfo* alloc_info);

		//device_memory(VkDevice d, VkDeviceSize size, uint32_t memory_type_index);
		//device_memory(VkDevice d, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags = 0);
	//	device_memory(VkDevice d, VkImage img, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags = 0); // dedicate allocation for VkImage object
	//	device_memory(VkDevice d, VkBuffer b, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags = 0); // dedicate allocation for VkBuffer object
		~device_memory();

		device_memory(device_memory&& other) noexcept;
		device_memory& operator = (device_memory&& other) noexcept;

		device_memory(const device_memory&) = delete;
		device_memory& operator = (const device_memory&) = delete;

		operator VkDeviceMemory() const { return mem; }
	};

}
