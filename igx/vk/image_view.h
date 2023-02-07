#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{

	class IGX_DLL image_view
	{
		VkImageView view = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		image_view() = default;
		image_view(VkDevice d, const VkImageViewCreateInfo * create_info);
		
		image_view(image_view&& other) noexcept;
		image_view& operator = (image_view&& other) noexcept;

		image_view(const image_view&) = delete;
		image_view& operator = (const image_view&) = delete;

		~image_view();

		operator const VkImageView& () const { return view; }
	};
}
