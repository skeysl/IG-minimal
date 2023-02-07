#include "fence.h"

#include <stdexcept>


namespace vk
{

	fence::fence(VkDevice d, VkFenceCreateFlags flags) : dev(d)
	{
		VkFenceCreateInfo create_info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		create_info.flags = flags; 

		VkResult err = vkCreateFence(dev, &create_info, NULL, &f);
		if (err != VK_SUCCESS)
		{
			throw std::runtime_error("vkCreateFence failed");
		}
	}



	fence::~fence()
	{
		if (f)
			vkDestroyFence(dev, f, NULL);
	}

	fence::fence(fence&& other) noexcept : f(other.f), dev(other.dev)
	{
		other.f = VK_NULL_HANDLE;
	}


	fence& fence::operator = (fence&& other) noexcept
	{
		std::swap(f, other.f);
		std::swap(dev, other.dev);
		return *this;
	}


	void fence::wait()
	{
		VkResult err = vkWaitForFences(dev, 1, &f, VK_TRUE, UINT64_MAX);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkWaitForFences failed!");
	}


	void fence::reset()
	{
		VkResult err = vkResetFences(dev, 1, &f);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkWaitForFences failed!");
	}

}
