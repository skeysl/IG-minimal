#include "semaphore.h"

#include <stdexcept>


namespace vk
{

	semaphore::semaphore(VkDevice d) : dev(d)
	{
		VkSemaphoreCreateInfo create_info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		
		VkResult err = vkCreateSemaphore(dev, &create_info,	NULL, &s);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateSemaphore failed");
	}


	semaphore::semaphore(VkDevice d, const VkSemaphoreCreateInfo * create_info) : dev(d)
	{
		VkResult err = vkCreateSemaphore(dev, create_info,	NULL, &s);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateSemaphore failed");
	}


	semaphore::~semaphore()
	{
		if (s)
			vkDestroySemaphore(dev, s, NULL);
	}

	semaphore::semaphore(semaphore && other) : s(other.s), dev(other.dev)
	{
		other.s = VK_NULL_HANDLE;
	}

	semaphore & semaphore::operator = (semaphore && other)
	{
		std::swap(s, other.s);
		std::swap(dev, other.dev);
		return *this;
	}
}
