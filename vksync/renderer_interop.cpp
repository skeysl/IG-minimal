#include "renderer_interop.h"

#include <igx/vk/device_memory_handle.h>

#include <ig/log.h>

#include <vulkan/vulkan_win32.h>


uint32_t find_memory_type(VkPhysicalDeviceMemoryProperties memory_properties, uint32_t memoryTypeBits, VkMemoryPropertyFlags memory_flags)
{
	IGLOG_TRACE("  finding memory type bits: " << std::bitset<16>(memoryTypeBits) << " memmory flags:" << memory_flags_to_str(memory_flags));
	for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & (1 << i)) &&
			((memory_properties.memoryTypes[i].propertyFlags & memory_flags) == memory_flags))
		{
			IGLOG_TRACE("  found at index " << i);
			return i;
		}
	}
	throw std::runtime_error("unsupported memory type!");
}


uint32_t find_memory_type(VkPhysicalDeviceMemoryProperties memory_properties, VkMemoryRequirements mem_rq, VkMemoryPropertyFlags memory_flags)
{
	return find_memory_type(memory_properties, mem_rq.memoryTypeBits, memory_flags);
}


uint32_t find_memory_type(VkPhysicalDeviceMemoryProperties memory_properties, VkMemoryRequirements2 mem_rq, VkMemoryPropertyFlags memory_flags)
{
	return find_memory_type(memory_properties, mem_rq.memoryRequirements.memoryTypeBits, memory_flags);
}


void image_memory_requirements(VkDevice dev, VkImage img, VkMemoryRequirements2& req)
	{
		VkImageMemoryRequirementsInfo2 img_info{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2 };
		img_info.image = img;

		vkGetImageMemoryRequirements2(dev, &img_info, &req);

		IGLOG_TRACE("image requirements " << mem2.memoryRequirements.size / 1024 << "KB"
			" alignment:" << mem2.memoryRequirements.alignment <<
			" memory type bits:" << std::bitset<16>(mem2.memoryRequirements.memoryTypeBits));
	}


renderer_interop::renderer_interop(VkInstance inst, VkPhysicalDevice gpu, unsigned w, unsigned h, unsigned swapchain_length) :
	width(w),
	height(h)
{
	VkResult err;

	// gpu properties and features

	VkPhysicalDevicePCIBusInfoPropertiesEXT gpu_pci_properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT };
	VkPhysicalDeviceProperties2 gpu_properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, &gpu_pci_properties };
		
	vkGetPhysicalDeviceProperties2(gpu, &gpu_properties);

	IGLOG(gpu_properties.properties.deviceName 
		<< " @ pci domain: " << gpu_pci_properties.pciDomain
		<< " bus: " << gpu_pci_properties.pciBus
		<< " device: " << gpu_pci_properties.pciDevice);

	VkPhysicalDeviceFeatures2 gpu_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	vkGetPhysicalDeviceFeatures2(gpu, &gpu_features);

	VkPhysicalDeviceMemoryProperties 	gpu_memory_properties;
	vkGetPhysicalDeviceMemoryProperties(gpu, &gpu_memory_properties);

	
	// device

	std::vector<const char*> device_extensions = {
	//	VK_EXT_PCI_BUS_INFO_EXTENSION_NAME,
		VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME, // interop
		VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME // interop
	};

	uint32_t queue_families_count;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_families_count, NULL);
	std::vector<VkQueueFamilyProperties> queue_families(queue_families_count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_families_count, queue_families.data());


	uint32_t queue_graphics_family = 0;
	for (uint32_t i = 0; i < queue_families.size(); i++)
	{
		if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
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
	
	VkDeviceCreateInfo device_create_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, &dev_features };
	device_create_info.queueCreateInfoCount = (uint32_t)queues.size();
	device_create_info.pQueueCreateInfos = queues.data();
	device_create_info.enabledExtensionCount = (uint32_t)device_extensions.size();
	device_create_info.ppEnabledExtensionNames = !device_extensions.empty() ? device_extensions.data() : NULL;
				

	dev = vk::device(gpu, &device_create_info);

	vkGetDeviceQueue(dev, queue_graphics_family, 0, &q0);
	if (q0 == VK_NULL_HANDLE)
		throw std::runtime_error("vkGetDeviceQueue failed");


	auto swp_format = VK_FORMAT_R8G8B8A8_UNORM;

	VkExternalMemoryImageCreateInfo swp_ci_ext{ VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO };
	swp_ci_ext.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;

	VkImageCreateInfo swp_ci = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, &swp_ci_ext };
	swp_ci.flags = 0;
	swp_ci.imageType = VK_IMAGE_TYPE_2D;
	swp_ci.format = swp_format;
	swp_ci.extent = { width, height, 1 };
	swp_ci.mipLevels = 1;
	swp_ci.arrayLayers = 1;
	swp_ci.samples = VK_SAMPLE_COUNT_1_BIT;
	swp_ci.tiling = VK_IMAGE_TILING_OPTIMAL;
	swp_ci.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swp_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swp_ci.queueFamilyIndexCount = 0;
	swp_ci.pQueueFamilyIndices = NULL;
	swp_ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	swp_img = vk::image(dev, &swp_ci);

	VkMemoryRequirements2 swp_img_mem_req{ VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2 };
	image_memory_requirements(dev, swp_img, swp_img_mem_req);

	auto memory_type_index = find_memory_type(gpu_memory_properties, swp_img_mem_req.memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	auto size = swp_img_mem_req.memoryRequirements.size;

	VkExportMemoryAllocateInfo export_alloc_info{ VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO };
	export_alloc_info.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
	VkMemoryAllocateInfo alloc_info{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, &export_alloc_info };
	alloc_info.allocationSize = swp_img_mem_req.memoryRequirements.size;
	alloc_info.memoryTypeIndex = memory_type_index;

	swp_img_mem = vk::device_memory(dev, &alloc_info);

	vkBindImageMemory(dev, swp_img, swp_img_mem, 0);

	auto swp_img_mem_handle = vk::get_memory_handle(dev, swp_img_mem);

	swp_mem_gl = gl::memory_object(
				size,
				swp_img_mem_handle, GL_HANDLE_TYPE_OPAQUE_WIN32_EXT);

	swp_tex_gl = gl::texture_2d(1, GL_RGBA8, width, height, swp_mem_gl);

	swp_fbo_gl = gl::framebuffer({ 
		gl::framebuffer::framebufer_texture_desc(GL_COLOR_ATTACHMENT0, swp_tex_gl)
	});

	
	

	VkImageViewCreateInfo view_info{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	view_info.image = swp_img;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = swp_format;

	view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	view_info.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

	swp_imgv = vk::image_view(dev, &view_info);
	
		
	create_render_pass(swp_format);

	create_framebuffer(w, h);

	
	tr = triangle_renderer(dev, rp, width, height);

	frame = interop::frame_sync_exported(dev, swp_tex_gl);

	for (size_t i = 0; i < swapchain_length; i++)
	{
		cmdb_pools.emplace_back(dev, 0, queue_graphics_family); 
		draw_frame_cmdb.emplace_back(dev, cmdb_pools.back());
		draw_completed_fences.emplace_back(dev, VK_FENCE_CREATE_SIGNALED_BIT);
	}
}



	



void renderer_interop::create_render_pass(VkFormat color_format)
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


void renderer_interop::create_framebuffer(int w, int h)
{
	VkImageView color_attachment = swp_imgv;

	VkFramebufferCreateInfo fb_create_info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	fb_create_info.renderPass = rp;
	fb_create_info.attachmentCount = 1;
	fb_create_info.pAttachments = &color_attachment;
	fb_create_info.width = w;
	fb_create_info.height = h;
	fb_create_info.layers = 1;

	output = vk::framebuffer(dev, &fb_create_info);
	
}


renderer_interop::~renderer_interop()
{
	vkDeviceWaitIdle(dev);
}


void renderer_interop::record(VkCommandBuffer cmdb, VkFramebuffer fb, float t, unsigned draw_calls_count)
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


void renderer_interop::run(unsigned draw_calls_count)
{
	VkResult err;

	frame.frame_begin();
	{
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

		record(cmdb, output, t, draw_calls_count);

		err = vkEndCommandBuffer(cmdb);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkQueuePresentKHR failed");


		// submit
		{
			VkSemaphore wait_semaphores[] = { frame.acquired_semaphore() };
			VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			VkSemaphore signal_semaphores[] = { frame.released_semaphore() };

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
	}
	frame.frame_end();

	glBlitNamedFramebuffer(
		swp_fbo_gl,
		0,
		0, 0, width, height,
		0, 0, width, height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	cmdb_index = (cmdb_index + 1) % draw_frame_cmdb.size();
	frame_index++;
}


void renderer_interop::wait_idle()
{
	vkDeviceWaitIdle(dev);
}

