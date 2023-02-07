#include "debug_utils_messenger.h"

#include <stdexcept>


namespace vk
{

	debug_utils_messenger::debug_utils_messenger(VkInstance inst, const VkDebugUtilsMessengerCreateInfoEXT * create_info) : 
		instance(inst)
	{
		auto CreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (CreateDebugUtilsMessenger)
		{
			VkResult err = CreateDebugUtilsMessenger(inst, create_info, NULL, &messenger);
			if (err != VK_SUCCESS)
				throw std::runtime_error("vkCreateDebugUtilsMessenger failed!");
		}
		else
		{
			throw std::runtime_error("CreateDebugUtilsMessenger not present");
		}
	}

	
	debug_utils_messenger::~debug_utils_messenger()
	{
		if (messenger)
		{
			auto DestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (DestroyDebugUtilsMessenger)
				DestroyDebugUtilsMessenger(instance, messenger, NULL);
		}
	}

	debug_utils_messenger::debug_utils_messenger(debug_utils_messenger&& other) noexcept :
		instance(other.instance),
		messenger(other.messenger)
	{
		other.messenger = VK_NULL_HANDLE;
	}

	debug_utils_messenger& debug_utils_messenger::operator = (debug_utils_messenger&& other) noexcept
	{
		std::swap(instance, other.instance);
		std::swap(messenger, other.messenger);
		return *this;
	}
}
