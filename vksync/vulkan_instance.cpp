#include "vulkan_instance.h"

#include "ig/log.h"

#include <stdexcept>


vulkan_instance::vulkan_instance(
	const std::vector<const char*>& instance_extensions, bool debug)
{
	std::vector<const char*> instance_layers;

	if (debug)
	{
		instance_layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	VkApplicationInfo app_info{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
	app_info.pApplicationName = NULL;
	app_info.applicationVersion = 1;
	app_info.pEngineName = NULL;
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo inst_info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledLayerCount = (uint32_t)instance_layers.size();
	inst_info.ppEnabledLayerNames = !instance_layers.empty() ? instance_layers.data() : nullptr;
	inst_info.enabledExtensionCount = (uint32_t)instance_extensions.size();
	inst_info.ppEnabledExtensionNames = !instance_extensions.empty() ? instance_extensions.data() : nullptr;

	inst = vk::instance(&inst_info);

	if (debug)
	{
		VkDebugUtilsMessengerCreateInfoEXT create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		create_info.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
		create_info.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info.pfnUserCallback =
			[](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) -> VkBool32
		{
			IGLOG(pCallbackData->pMessage);	return VK_FALSE;
		};

		debug_messenger = vk::debug_utils_messenger(inst, &create_info);
	}

	VkResult res;

	uint32_t physical_devices_count;
	res = vkEnumeratePhysicalDevices(inst, &physical_devices_count, NULL);
	if (res != VK_SUCCESS)
		throw std::runtime_error("vkEnumeratePhysicalDevices failed");
	physical_devices.resize(physical_devices_count);
	res = vkEnumeratePhysicalDevices(inst, &physical_devices_count, physical_devices.data());
	if (res != VK_SUCCESS)
		throw std::runtime_error("vkEnumeratePhysicalDevices failed");

	IGLOG("physical devices count: " << physical_devices.size());
}


