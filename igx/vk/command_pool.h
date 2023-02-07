#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL command_pool
	{
	public:

		command_pool() = default;

		command_pool(VkDevice d, VkCommandPoolCreateFlags flags, uint32_t queue_family_index);
		~command_pool();

		command_pool(command_pool&& other) noexcept;
		command_pool& operator = (command_pool&& other) noexcept;


		command_pool(const command_pool &) = delete;
		command_pool & operator = (const command_pool &) = delete;

		operator VkCommandPool() const { return cmd_pool; }

		void reset(VkCommandPoolResetFlags flags = 0);

	protected:

		VkCommandPool cmd_pool = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
	};
}
