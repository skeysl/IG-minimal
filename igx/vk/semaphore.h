#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL semaphore
	{
	public:

		semaphore() = default;

		semaphore(VkDevice d);
		semaphore(VkDevice d, const VkSemaphoreCreateInfo* create_info);
		~semaphore();

		semaphore(semaphore && other);
		semaphore & operator = (semaphore && other);

		semaphore(const semaphore &) = delete;
		semaphore & operator = (const semaphore &) = delete;

		operator const VkSemaphore & () const { return s; }
	
	protected:

		VkSemaphore s = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
	};

}
