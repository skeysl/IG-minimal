#include "image.h"

#include <stdexcept>


namespace vk
{
	image::image(VkDevice d, const VkImageCreateInfo * create_info) : dev(d)
	{
		VkResult err = vkCreateImage(dev, create_info, NULL, &img);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateImageView failed");
	}

	

	image::image(image&& other) noexcept : img(other.img), dev(other.dev)
	{
		other.img = VK_NULL_HANDLE;
	}

	image& image::operator = (image&& other) noexcept
	{
		std::swap(img, other.img);
		std::swap(dev, other.dev);
		return *this;
	}


	image::~image()
	{
		if (img != VK_NULL_HANDLE)
			vkDestroyImage(dev, img, NULL);
	}
}
