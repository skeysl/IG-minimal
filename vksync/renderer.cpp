#include "renderer.h"

#include <ig/log.h>

#include <vulkan/vulkan_win32.h>


std::vector<VkExtensionProperties> enumerate_device_extension_properties(VkPhysicalDevice gpu)
{
	uint32_t count;
	vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, nullptr);

	if (count > 0)
	{
		std::vector<VkExtensionProperties> ext(count);
		vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, ext.data());

		return ext;
	}
	return {};
}


bool extension_supported(const std::vector<VkExtensionProperties> & extensions, const char* ext)
	{
		auto ext_it = std::find_if(
			extensions.begin(),
			extensions.end(),
			[&](const auto& e) { return strcmp(e.extensionName, ext) == 0; });
		return (ext_it != extensions.end());
	}


renderer::renderer(VkInstance inst, VkPhysicalDevice gpu, VkSurfaceKHR surf, unsigned w, unsigned h, VkPresentModeKHR present_mode, unsigned swapchain_length) :
	width(w),
	height(h),
	surface(surf)
{
	VkResult err;

	auto extensions = enumerate_device_extension_properties(gpu);

	
	bool nv_present_barrier_supported = extension_supported(extensions, VK_NV_PRESENT_BARRIER_EXTENSION_NAME);

	// gpu properties and features

	VkPhysicalDevicePCIBusInfoPropertiesEXT gpu_pci_properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT };
	VkPhysicalDeviceProperties2 gpu_properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, &gpu_pci_properties };
		
	vkGetPhysicalDeviceProperties2(gpu, &gpu_properties);

	IGLOG(gpu_properties.properties.deviceName 
		<< " @ pci domain: " << gpu_pci_properties.pciDomain
		<< " bus: " << gpu_pci_properties.pciBus
		<< " device: " << gpu_pci_properties.pciDevice);

	VkPhysicalDeviceFeatures2 gpu_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	VkPhysicalDevicePresentBarrierFeaturesNV gpu_present_barrier_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_BARRIER_FEATURES_NV };
	
	if (nv_present_barrier_supported)
		gpu_features.pNext = &gpu_present_barrier_features;

	vkGetPhysicalDeviceFeatures2(gpu, &gpu_features);

	if (nv_present_barrier_supported)
	{
		IGLOG("gpu present barrier supported: " << (int)gpu_present_barrier_features.presentBarrier);
	}

	// surface capabilities

	VkSurfaceCapabilities2KHR surface_capabilities{ VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR };
	VkSurfaceCapabilitiesPresentBarrierNV surface_present_barrier_capabilities{ VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_PRESENT_BARRIER_NV };

	if (nv_present_barrier_supported)
		surface_capabilities.pNext = &surface_present_barrier_capabilities;

	VkPhysicalDeviceSurfaceInfo2KHR surface_info{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR, nullptr, surface };

	PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR fnGetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)vkGetInstanceProcAddr(inst, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
	err = fnGetPhysicalDeviceSurfaceCapabilities2KHR(gpu, &surface_info, &surface_capabilities);
	if (err == VK_SUCCESS)
	{
		if (nv_present_barrier_supported)
		{
			IGLOG("surface present barrier supported: " << (int)surface_present_barrier_capabilities.presentBarrierSupported);
		}
	}
	else
		throw std::runtime_error("vkGetPhysicalDeviceSurfaceCapabilities2KHR failed!");
	

	


	uint32_t surface_formats_count;
	err = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &surface_formats_count, NULL);
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetPhysicalDeviceSurfaceFormatsKHR failed!");
	std::vector<VkSurfaceFormatKHR> surface_formats(surface_formats_count);
	err = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &surface_formats_count, surface_formats.data());
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetPhysicalDeviceSurfaceFormatsKHR failed!");

	uint32_t present_modes_count;
	err = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_modes_count, NULL);
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetPhysicalDeviceSurfacePresentModesKHR failed!");
	std::vector<VkPresentModeKHR> present_modes(present_modes_count);
	err = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_modes_count, present_modes.data());
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetPhysicalDeviceSurfacePresentModesKHR failed!");

	/*for (auto pm : present_modes)
	{
		switch (pm)
		{
		case  VK_PRESENT_MODE_IMMEDIATE_KHR: IGLOG("immediate"); break;
		case VK_PRESENT_MODE_MAILBOX_KHR: IGLOG("mailbox"); break;
		case VK_PRESENT_MODE_FIFO_KHR: IGLOG("fifo"); break;
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR: IGLOG("fifo relaxed"); break;
		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR: IGLOG("shared"); break;
		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:IGLOG("shared continuous"); break;
		default:
			IGLOG("unknown");
		}
	}*/

	auto surface_format_it = std::find_if(
			surface_formats.begin(),
			surface_formats.end(), [](const VkSurfaceFormatKHR& f) { return f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; });

	if (surface_format_it != surface_formats.end())
		swapchain_format = *surface_format_it;
	else
		throw std::runtime_error("Incompatible vulkan surface, no suitable color format found!");
		

	
	// device

	std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
	};

	if (nv_present_barrier_supported)
		device_extensions.push_back(VK_NV_PRESENT_BARRIER_EXTENSION_NAME);

	uint32_t queue_families_count;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_families_count, NULL);
	std::vector<VkQueueFamilyProperties> queue_families(queue_families_count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_families_count, queue_families.data());


	uint32_t queue_graphics_family = 0;
	for (uint32_t i = 0; i < queue_families.size(); i++)
	{
		VkBool32 supported;
		vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &supported);

		if ((supported == VK_TRUE) && (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			queue_graphics_family = i;
			break;
		}
	}
		
	float queue_priorities[1] = { 1.0 };

	std::vector<VkDeviceQueueCreateInfo> queues(1, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO });
	queues[0].queueFamilyIndex = queue_graphics_family;
	queues[0].queueCount = 1; 
	queues[0].pQueuePriorities = queue_priorities;

	
	VkPhysicalDeviceFeatures2 dev_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	VkPhysicalDevicePresentBarrierFeaturesNV dev_present_barrier_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_BARRIER_FEATURES_NV };

	if ((nv_present_barrier_supported) && (gpu_present_barrier_features.presentBarrier))
	{
		dev_features.pNext = &dev_present_barrier_features;
		dev_present_barrier_features.presentBarrier = VK_TRUE;
	}	
	
	VkDeviceCreateInfo device_create_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, &dev_features };
	device_create_info.queueCreateInfoCount = (uint32_t)queues.size();
	device_create_info.pQueueCreateInfos = queues.data();
	device_create_info.enabledExtensionCount = (uint32_t)device_extensions.size();
	device_create_info.ppEnabledExtensionNames = !device_extensions.empty() ? device_extensions.data() : NULL;
				

	dev = vk::device(gpu, &device_create_info);

	vkGetDeviceQueue(dev, queue_graphics_family, 0, &q0);
	if (q0 == VK_NULL_HANDLE)
		throw std::runtime_error("vkGetDeviceQueue failed");


	swapchain_length = std::max(surface_capabilities.surfaceCapabilities.minImageCount, swapchain_length);
	if (surface_capabilities.surfaceCapabilities.maxImageCount > 0)
		swapchain_length = std::min(swapchain_length, surface_capabilities.surfaceCapabilities.maxImageCount);

	transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		(surface_capabilities.surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) ?
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : surface_capabilities.surfaceCapabilities.currentTransform;

	create_swapchain(surface, swapchain_format, w, h, present_mode, swapchain_length, transform, dev_present_barrier_features.presentBarrier);
	
	create_render_pass(swapchain_format.format);

	for (const auto& imgv : swapchain_views)
	{
		VkImageView color_attachment = imgv;

		VkFramebufferCreateInfo fb_create_info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		fb_create_info.renderPass = rp;
		fb_create_info.attachmentCount = 1;
		fb_create_info.pAttachments = &color_attachment;
		fb_create_info.width = w;
		fb_create_info.height = h;
		fb_create_info.layers = 1;

		outputs.emplace_back(dev, &fb_create_info);
	}
		
	tr = triangle_renderer(dev, rp, width, height);

	image_acquired_semaphore = vk::semaphore(dev);
	draw_completed_semaphore = vk::semaphore(dev);

	for (size_t i = 0; i < swapchain_views.size(); i++)
	{
		cmdb_pools.emplace_back(dev, 0, queue_graphics_family); 
		draw_frame_cmdb.emplace_back(dev, cmdb_pools.back());
		draw_completed_fences.emplace_back(dev, VK_FENCE_CREATE_SIGNALED_BIT);
	}
}


void renderer::create_swapchain(VkSurfaceKHR surface, VkSurfaceFormatKHR swapchain_format, unsigned w, unsigned h, VkPresentModeKHR present_mode, unsigned swapchain_length, VkSurfaceTransformFlagBitsKHR transform ,
	VkBool32 present_barrier_enable) 
{
	old_swapchain = std::move(swp);

	VkSwapchainCreateInfoKHR swapchain_create_info{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapchain_create_info.surface = surface;
	swapchain_create_info.minImageCount = swapchain_length;
	swapchain_create_info.imageFormat = swapchain_format.format;
	swapchain_create_info.imageColorSpace = swapchain_format.colorSpace;
	swapchain_create_info.imageExtent = { w, h };
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 0;
	swapchain_create_info.pQueueFamilyIndices = NULL;
	swapchain_create_info.preTransform = transform;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.presentMode = present_mode;
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.oldSwapchain = old_swapchain;

	VkSwapchainPresentBarrierCreateInfoNV swapchain_pb_info{ VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_BARRIER_CREATE_INFO_NV };

	if (present_barrier_enable)
	{
		swapchain_create_info.pNext = &swapchain_pb_info;
		swapchain_pb_info.presentBarrierEnable = present_barrier_enable;
	}
		
	swp = vk::swapchain(dev, &swapchain_create_info);
		
	uint32_t swapchain_images_count;
	VkResult err = vkGetSwapchainImagesKHR(dev, swp, &swapchain_images_count, NULL);
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetSwapchainImagesKHR failed");
	std::vector<VkImage> swapchain_images(swapchain_images_count);
	err = vkGetSwapchainImagesKHR(dev, swp, &swapchain_images_count, swapchain_images.data());
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkGetSwapchainImagesKHR failed");

	swapchain_views.clear();
	for (const auto & img : swapchain_images)
	{
		VkImageViewCreateInfo view_info{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		view_info.image = img;
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = swapchain_format.format;

		view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		view_info.subresourceRange =  { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

		swapchain_views.emplace_back(dev, &view_info);
	}
}


void renderer::create_render_pass(VkFormat color_format)
{
	VkAttachmentDescription2 color_attachment{ VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2 };
	color_attachment.format = color_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	
	VkAttachmentReference2 color_reference{ VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2, nullptr, 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT };

	VkSubpassDescription2 subpass_description{ VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2 };
	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_reference;

	VkSubpassDependency2 dependency{ VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2 };
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = /*VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |*/ VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo2 rp_info{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2 };
	rp_info.attachmentCount = 1;
	rp_info.pAttachments = &color_attachment;
	rp_info.subpassCount = 1;
	rp_info.pSubpasses = &subpass_description;
	rp_info.dependencyCount = 1;
	rp_info.pDependencies = &dependency;
	
	rp = vk::render_pass(dev, &rp_info);
}



renderer::~renderer()
{
	vkDeviceWaitIdle(dev);
}


void renderer::record(VkCommandBuffer cmdb, VkFramebuffer fb, float t, unsigned draw_calls_count)
{
	VkClearValue clear_values[1];
	clear_values[0].color.float32[0] = 0.1f;
	clear_values[0].color.float32[1] = 0.2f;
	clear_values[0].color.float32[2] = 0.3f;
	clear_values[0].color.float32[3] = 0.0f;

	VkRenderPassBeginInfo rp_begin{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	rp_begin.renderPass = rp;
	rp_begin.framebuffer = fb;
	rp_begin.renderArea = { { 0u, 0u }, { width, height } };
	rp_begin.clearValueCount = 1;
	rp_begin.pClearValues = clear_values;

	vkCmdBeginRenderPass(cmdb, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

	for (int i = 0; i < draw_calls_count; i++)
		tr.record(cmdb, t);

	vkCmdEndRenderPass(cmdb);
}


void renderer::run(unsigned draw_calls_count)
{
	uint32_t image_index;
	VkResult err = vkAcquireNextImageKHR(dev, swp, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &image_index);
	if (err != VK_SUCCESS)
		throw std::runtime_error("vkAcquireNextImageKHR failed");

	draw_completed_fences[cmdb_index].wait();
	draw_completed_fences[cmdb_index].reset();
	
	float t = timer.elapsed();

	cmdb_pools[cmdb_index].reset();

	VkCommandBuffer cmdb = draw_frame_cmdb[cmdb_index];

	// record

	VkCommandBufferBeginInfo begin_info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	err = vkBeginCommandBuffer(cmdb, &begin_info);
	if (err != VK_SUCCESS)
			throw std::runtime_error("vkQueuePresentKHR failed");
	
	record(cmdb, outputs[image_index], t, draw_calls_count);
	
	err = vkEndCommandBuffer(cmdb);
	if (err != VK_SUCCESS)
			throw std::runtime_error("vkQueuePresentKHR failed");
	
	// submit
	{
		VkSemaphore wait_semaphores[] = { image_acquired_semaphore };
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSemaphore signal_semaphores[] = { draw_completed_semaphore };

		VkSubmitInfo si{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
		si.commandBufferCount = 1;
		si.pCommandBuffers = &cmdb;

		si.waitSemaphoreCount = 1;
		si.pWaitSemaphores = wait_semaphores;
		si.pWaitDstStageMask = wait_stages;

		si.signalSemaphoreCount = 1;
		si.pSignalSemaphores = signal_semaphores;

		VkResult err = vkQueueSubmit(q0, 1, &si, draw_completed_fences[cmdb_index]);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkQueueSubmit failed");
	}
	

	// present
	{
		VkSwapchainKHR swapchains[] = { swp };
		uint32_t image_indices[] = { image_index };

		VkSemaphore wait_semaphores[] = { draw_completed_semaphore };
				
		VkPresentInfoKHR present{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		present.swapchainCount = 1;
		present.pSwapchains = swapchains;
		present.pImageIndices = image_indices;
		present.waitSemaphoreCount = 1;
		present.pWaitSemaphores = wait_semaphores;

		VkResult err = vkQueuePresentKHR(q0, &present);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkQueuePresentKHR failed");
	}

	cmdb_index = (cmdb_index + 1) % draw_frame_cmdb.size();
	frame_index++;
}


void renderer::wait_idle()
{
	vkDeviceWaitIdle(dev);
}

