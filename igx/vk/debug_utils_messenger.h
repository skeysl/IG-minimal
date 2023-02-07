#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{

	class IGX_DLL debug_utils_messenger
	{
	public:
			
		debug_utils_messenger() = default;
		debug_utils_messenger(VkInstance inst, const VkDebugUtilsMessengerCreateInfoEXT * create_info);

		~debug_utils_messenger();

		debug_utils_messenger(const debug_utils_messenger &) = delete;
		debug_utils_messenger(debug_utils_messenger &&) noexcept;

		debug_utils_messenger & operator = (const debug_utils_messenger &) = delete;
		debug_utils_messenger & operator = (debug_utils_messenger &&) noexcept;

	protected:

		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
	};
}
