#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>

#include <utility>

namespace vk
{
	class IGX_DLL instance
	{
	public:

		instance() : inst(VK_NULL_HANDLE) {}
		
		instance(const VkInstanceCreateInfo* create_info);
		~instance();

		instance(instance && other) noexcept : inst(other.inst)
		{
			other.inst = VK_NULL_HANDLE;
		}

		instance & operator = (instance && other) noexcept
		{
			std::swap(inst, other.inst); 
			return *this;
		}


		instance(const instance &) = delete;
		instance & operator = (const instance &) = delete;
		
		operator VkInstance() const { return inst; }

	protected:

		VkInstance inst;
	};
}
