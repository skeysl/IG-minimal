#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{

	class IGX_DLL image
	{
		VkImage img = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		image() = default;
		image(VkDevice d, const VkImageCreateInfo * create_info);
		
		image(image&& other) noexcept;
		image& operator = (image&& other) noexcept;

		image(const image&) = delete;
		image& operator = (const image&) = delete;

		~image();

		operator const VkImage& () const { return img; }
	};
}
