#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL fence
	{
		VkFence f = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		fence() = default;

		fence(VkDevice d, VkFenceCreateFlags flags);
		~fence();

		fence(fence && other) noexcept;
		fence & operator = (fence && other) noexcept;

		fence(const fence &) = delete;
		fence & operator = (const fence &) = delete;

		operator const VkFence & () const { return f; }

		void wait();
		void reset();
	};
}
