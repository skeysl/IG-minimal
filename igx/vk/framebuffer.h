#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{

	class IGX_DLL framebuffer
	{
		VkFramebuffer fb = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		framebuffer() = default;

		framebuffer(VkDevice d, const VkFramebufferCreateInfo* info);
		~framebuffer();

		framebuffer(framebuffer&& other) noexcept;
		framebuffer& operator = (framebuffer&& other) noexcept;

		framebuffer(const framebuffer&) = delete;
		framebuffer& operator = (const framebuffer&) = delete;
		
		operator VkFramebuffer() const { return fb; }
	};

}
