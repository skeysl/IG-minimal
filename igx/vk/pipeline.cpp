#include "pipeline.h"

#include <stdexcept>


namespace vk
{
	pipeline::pipeline(VkDevice d, VkPipelineCache cache, const VkGraphicsPipelineCreateInfo* create_info) : dev(d)
	{
		VkResult err = vkCreateGraphicsPipelines(dev, cache, 1, create_info, NULL, &p);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateGraphicsPipelines failed");
	}


	pipeline::~pipeline()
	{
		if (p)
			vkDestroyPipeline(dev, p, NULL);
	}

	pipeline::pipeline(pipeline&& other) noexcept : p(other.p), dev(other.dev)
	{
		other.p = VK_NULL_HANDLE;
	}

	pipeline& pipeline::operator = (pipeline&& other) noexcept
	{
		std::swap(dev, other.dev);
		std::swap(p, other.p);
		return *this;
	}





}
