#include "instance.h"

#include <stdexcept>


namespace vk
{
	instance::instance(const VkInstanceCreateInfo * create_info)
	{
		VkResult err = vkCreateInstance(create_info, nullptr, &inst);
		if (err != VK_SUCCESS)
		{
			if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
				throw std::runtime_error("Cannot find a compatible Vulkan installable client driver "
					"(ICD).\n\nPlease look at the Getting Started guide for "
					"additional information.\n"
					"vkCreateInstance Failure");
			}
			else if (err == VK_ERROR_EXTENSION_NOT_PRESENT) {
				throw std::runtime_error("Cannot find a specified extension library"
					".\nMake sure your layers path is set appropriately\n"
					"vkCreateInstance Failure");
			}
			else if (err == VK_ERROR_LAYER_NOT_PRESENT) {
				throw std::runtime_error("VK_ERROR_LAYER_NOT_PRESENT\n"
					"vkCreateInstance Failure");
			}
			else
				throw std::runtime_error("vkCreateInstance failed.\n\nDo you have a compatible Vulkan "
					"installable client driver (ICD) installed?\nPlease look at "
					"the Getting Started guide for additional information.\n"
					"vkCreateInstance Failure");
		}
	}


	instance::~instance()
	{
		if (inst)
			vkDestroyInstance(inst, nullptr);
	}
}
