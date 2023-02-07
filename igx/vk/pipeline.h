#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	
	class IGX_DLL pipeline
	{
	public:

		pipeline() = default;

		pipeline(VkDevice d, VkPipelineCache cache, const VkGraphicsPipelineCreateInfo * createInfo);
		~pipeline();

		pipeline(pipeline&& other) noexcept;
		pipeline& operator = (pipeline&& other) noexcept;

		pipeline(const pipeline &) = delete;
		pipeline & operator = (const pipeline &) = delete;

		operator const VkPipeline & () const { return p; }

	protected:

		VkPipeline p = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
	};
}
