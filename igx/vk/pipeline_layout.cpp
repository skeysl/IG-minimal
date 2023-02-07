#include "pipeline_layout.h"

#include <stdexcept>


namespace vk
{
	pipeline_layout::pipeline_layout(VkDevice d, const VkPipelineLayoutCreateInfo* create_info) : dev(d)
	{
		VkResult err = vkCreatePipelineLayout(dev, create_info, NULL, &pl);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreatePipelineLayout failed");
	}

	pipeline_layout::~pipeline_layout()
	{
		if (pl)
			vkDestroyPipelineLayout(dev, pl, NULL);
	}

	pipeline_layout::pipeline_layout(pipeline_layout&& other) noexcept : pl(other.pl), dev(other.dev)
	{
		other.pl = VK_NULL_HANDLE;
	}

	pipeline_layout& pipeline_layout::operator = (pipeline_layout&& other) noexcept
	{
		std::swap(dev, other.dev);
		std::swap(pl, other.pl);
		return *this;
	}

	
}
