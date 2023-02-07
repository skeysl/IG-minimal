#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>



namespace vk
{
	class IGX_DLL render_pass
	{
		VkRenderPass pass = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
		
	public:

		render_pass() = default;
		render_pass(VkDevice d, const VkRenderPassCreateInfo2 *);
		~render_pass();

		render_pass(render_pass&& other) noexcept;
		render_pass& operator = (render_pass&& other) noexcept;

		render_pass(const render_pass &) = delete;
		render_pass & operator = (const render_pass &) = delete;

		operator const VkRenderPass & () const { return pass; }
	};
}
