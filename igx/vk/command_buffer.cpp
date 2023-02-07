#include "command_buffer.h"


#include <stdexcept>


namespace vk
{

	command_buffer::command_buffer(VkDevice d, VkCommandPool p, VkCommandBufferLevel level) : dev(d), pool(p)
	{
		VkCommandBufferAllocateInfo cmd{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		cmd.commandPool = pool;
		cmd.level = level;
		cmd.commandBufferCount = 1;

		VkResult err = vkAllocateCommandBuffers(dev, &cmd, &cmdb);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkAllocateCommandBuffers failed!");
	}


	command_buffer::~command_buffer()
	{
		if (cmdb)
			vkFreeCommandBuffers(dev, pool, 1, &cmdb);
	}


	command_buffer::command_buffer(command_buffer && other) noexcept : cmdb(other.cmdb), dev(other.dev), pool(other.pool)
	{
		other.cmdb = VK_NULL_HANDLE;
	}

	command_buffer & command_buffer::operator = (command_buffer && other) noexcept
	{
		std::swap(cmdb, other.cmdb);
		std::swap(dev, other.dev);
		std::swap(pool, other.pool);
		return *this;
	}
}
