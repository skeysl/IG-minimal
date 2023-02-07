#include "command_pool.h"

#include <stdexcept>

namespace vk
{
	command_pool::command_pool(VkDevice d, VkCommandPoolCreateFlags flags, uint32_t queue_family_index) : dev(d)
	{
		VkCommandPoolCreateInfo cmd_pool_info{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		cmd_pool_info.flags = flags;
		cmd_pool_info.queueFamilyIndex = queue_family_index;
		
		VkResult err = vkCreateCommandPool(dev, &cmd_pool_info, NULL, &cmd_pool);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateCommandPool failed!");
	}

	command_pool::~command_pool()
	{
		if (cmd_pool)
			vkDestroyCommandPool(dev, cmd_pool, NULL);
	}

	command_pool::command_pool(command_pool&& other) noexcept : cmd_pool(other.cmd_pool), dev(other.dev)
	{
		other.cmd_pool = VK_NULL_HANDLE;
	}


	command_pool& command_pool::operator = (command_pool&& other) noexcept
	{
		std::swap(cmd_pool, other.cmd_pool);
		std::swap(dev, other.dev);
		return *this;
	}

	void command_pool::reset(VkCommandPoolResetFlags flags)
	{
		vkResetCommandPool(dev, cmd_pool, flags);
	}

}
