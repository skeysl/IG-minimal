#include "render_pass.h"

#include <stdexcept>


namespace vk
{
	render_pass::render_pass(VkDevice d, const VkRenderPassCreateInfo2 * create_info) : dev(d)
	{
		VkResult err = vkCreateRenderPass2(dev, create_info, NULL, &pass);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateRenderPass failed");
	}

	render_pass::~render_pass()
	{
		if (pass)
			vkDestroyRenderPass(dev, pass, NULL);
	}

	render_pass::render_pass(render_pass&& other) noexcept : 
		pass(other.pass), 
		dev(other.dev)
	{
		other.pass = VK_NULL_HANDLE;
	}

	render_pass& render_pass::operator = (render_pass&& other) noexcept
	{
		std::swap(pass, other.pass);
		std::swap(dev, other.dev);
		return *this;
	}
}