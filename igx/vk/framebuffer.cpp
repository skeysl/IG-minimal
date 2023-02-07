#include "framebuffer.h"

#include <stdexcept>


namespace vk
{
	framebuffer::framebuffer(VkDevice d, const VkFramebufferCreateInfo* info) : dev(d)
	{
		VkResult err = vkCreateFramebuffer(dev, info, NULL, &fb);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateFramebuffer failed");
	}

	framebuffer::framebuffer(framebuffer&& other) noexcept : fb(other.fb), dev(other.dev)
	{
		other.fb = VK_NULL_HANDLE;
	}

	framebuffer& framebuffer::operator = (framebuffer&& other) noexcept
	{
		std::swap(fb, other.fb);
		std::swap(dev, other.dev);
		return *this;
	}


	framebuffer::~framebuffer()
	{
		if (fb != VK_NULL_HANDLE)
			vkDestroyFramebuffer(dev, fb, NULL);
	}

}
