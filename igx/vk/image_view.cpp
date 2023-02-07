#include "image_view.h"

#include <stdexcept>


namespace vk
{
	image_view::image_view(VkDevice d, const VkImageViewCreateInfo * create_info) : dev(d)
	{
		VkResult err = vkCreateImageView(dev, create_info, NULL, &view);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateImageView failed");
	}

	

	image_view::image_view(image_view&& other) noexcept : view(other.view), dev(other.dev)
	{
		other.view = VK_NULL_HANDLE;
	}

	image_view& image_view::operator = (image_view&& other) noexcept
	{
		std::swap(view, other.view);
		std::swap(dev, other.dev);
		return *this;
	}


	image_view::~image_view()
	{
		if (view != VK_NULL_HANDLE)
			vkDestroyImageView(dev, view, NULL);
	}
}
