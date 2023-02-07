#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL pipeline_layout
	{
		VkPipelineLayout pl = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;

	public:

		pipeline_layout() = default;
		pipeline_layout(VkDevice d, const VkPipelineLayoutCreateInfo* create_info);
		~pipeline_layout();

		pipeline_layout(pipeline_layout&& other) noexcept;

		pipeline_layout& operator = (pipeline_layout&& other) noexcept;

		pipeline_layout(const pipeline_layout &) = delete;
		pipeline_layout & operator = (const pipeline_layout &) = delete;


		operator const VkPipelineLayout & () const { return pl; }
	};
}
