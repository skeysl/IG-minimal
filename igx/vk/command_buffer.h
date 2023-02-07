#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL command_buffer
	{
		VkCommandBuffer cmdb = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
		VkCommandPool pool = VK_NULL_HANDLE;

	public:

		command_buffer() = default;
		command_buffer(VkDevice d, VkCommandPool p, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~command_buffer();

		command_buffer(command_buffer && other) noexcept;
		command_buffer & operator = (command_buffer && other) noexcept;

		command_buffer(const command_buffer &) = delete;
		command_buffer & operator = (const command_buffer &) = delete;

		operator const VkCommandBuffer & () const { return cmdb; }
	};
}
