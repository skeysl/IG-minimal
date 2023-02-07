#include "device_memory.h"

#include <stdexcept>


namespace vk
{

	device_memory::device_memory(VkDevice d, const VkMemoryAllocateInfo* alloc_info) : dev(d)
	{
		VkResult err = vkAllocateMemory(dev, alloc_info, NULL, &mem);
		if (err != VK_SUCCESS)
		{
			switch (err)
			{
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				throw std::runtime_error("vkAllocateMemory failed, VK_ERROR_OUT_OF_HOST_MEMORY");
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				throw std::runtime_error("vkAllocateMemory failed, VK_ERROR_OUT_OF_DEVICE_MEMORY");
			case VK_ERROR_INVALID_EXTERNAL_HANDLE:
				throw std::runtime_error("vkAllocateMemory failed, VK_ERROR_INVALID_EXTERNAL_HANDLE");
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR:
				throw std::runtime_error("vkAllocateMemory failed, VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR");
			default:
				throw std::runtime_error("vkAllocateMemory failed, unknown reason.");
			}
		}
	}



	/*device_memory::device_memory(VkDevice d, const VkMemoryAllocateInfo & alloc_info) : dev(d)
	{
		VkResult err = vkAllocateMemory(dev, &alloc_info, NULL, &mem);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkAllocateMemory failed");
	}*/

	/*device_memory::device_memory(VkDevice d, VkDeviceSize size, uint32_t memory_type_index) : dev(d)
	{
		IGLOG_TRACE("vkAllocateMemory size:" << size/1024 << "KB memoryType: " << memory_type_index);

		VkMemoryAllocateInfo mem_alloc{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		mem_alloc.allocationSize = size;
		mem_alloc.memoryTypeIndex = memory_type_index;

		VkResult err = vkAllocateMemory(dev, &mem_alloc, NULL, &mem);
		if (err != VK_SUCCESS)
		{
			std::ostringstream s;
			s << "vkAllocateMemory failed, size:" << size/1024 << "KB memoryType: " << memory_type_index;
			throw std::runtime_error(s.str());
		}
	}*/

	/*device_memory::device_memory(VkDevice d, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags) : dev(d)
	{
		IGLOG_TRACE("vkAllocateMemory size:" << size / 1024 << "KB memoryType: " << memory_type_index);

		structure_chain<
			VkMemoryAllocateInfo,
			VkMemoryAllocateFlagsInfo> alloc_info;

		alloc_info.get<VkMemoryAllocateInfo>().allocationSize = size;
		alloc_info.get<VkMemoryAllocateInfo>().memoryTypeIndex = memory_type_index;
		alloc_info.get<VkMemoryAllocateFlagsInfo>().flags = flags;

		VkResult err = vkAllocateMemory(dev, &alloc_info.get<VkMemoryAllocateInfo>(), NULL, &mem);
		if (err != VK_SUCCESS)
		{
			std::ostringstream s;
			s << "vkAllocateMemory failed, size:" << size / 1024 << "KB memoryType: " << memory_type_index;
			throw std::runtime_error(s.str());
		}
	}*/

	/*device_memory::device_memory(VkDevice d, VkImage img, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags) : dev(d)
	{
		IGLOG_TRACE("vkAllocateMemory (img, dedicate) size:" << size/1024 << "KB memoryType: " << memory_type_index);

		VkMemoryAllocateFlagsInfo alloc_frags{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO };
		alloc_frags.flags = flags;

		VkMemoryDedicatedAllocateInfo dedicated{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO };
		dedicated.pNext = &alloc_frags;
		dedicated.image = img;
		dedicated.buffer = VK_NULL_HANDLE;

		VkMemoryAllocateInfo mem_alloc{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		mem_alloc.pNext = &dedicated;
		mem_alloc.allocationSize = size;
		mem_alloc.memoryTypeIndex = memory_type_index;

		VkResult err = vkAllocateMemory(dev, &mem_alloc, NULL, &mem);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkAllocateMemory failed");
	}

	device_memory::device_memory(VkDevice d, VkBuffer b, VkDeviceSize size, uint32_t memory_type_index, VkMemoryAllocateFlags flags) : dev(d)
	{
		IGLOG_TRACE("vkAllocateMemory (buffer, dedicate) size:" << size/1024 << "KB memoryType: " << memory_type_index);

		VkMemoryAllocateFlagsInfo alloc_frags{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO };
		alloc_frags.flags = flags;

		VkMemoryDedicatedAllocateInfo dedicated{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO };
		dedicated.pNext = &alloc_frags;
		dedicated.image = VK_NULL_HANDLE;
		dedicated.buffer = b;

		VkMemoryAllocateInfo mem_alloc{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		mem_alloc.pNext = &dedicated;
		mem_alloc.allocationSize = size;
		mem_alloc.memoryTypeIndex = memory_type_index;

		VkResult err = vkAllocateMemory(dev, &mem_alloc, NULL, &mem);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkAllocateMemory failed");
	}*/



	device_memory::~device_memory()
	{
		if (mem != VK_NULL_HANDLE)
		{
			vkFreeMemory(dev, mem, NULL);
		}
	}

	device_memory::device_memory(device_memory&& other) noexcept : mem(other.mem), dev(other.dev)
	{
		other.mem = VK_NULL_HANDLE;
	}

	device_memory& device_memory::operator = (device_memory&& other) noexcept
	{
		std::swap(mem, other.mem);
		std::swap(dev, other.dev);
		return *this;
	}
}
