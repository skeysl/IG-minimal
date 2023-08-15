#pragma once

#include <vulkan/vulkan.h>

#include <igx/vk/device.h>
#include <igx/vk/swapchain.h>
#include <igx/vk/image_view.h>
#include <igx/vk/render_pass.h>
#include <igx/vk/framebuffer.h>
#include <igx/vk/semaphore.h>
#include <igx/vk/fence.h>
#include <igx/vk/command_pool.h>
#include <igx/vk/command_buffer.h>

#include "triangle_renderer.h"

#include <ig/timer.h>


class renderer
{
public:

	renderer(VkInstance ins, VkPhysicalDevice pd, VkSurfaceKHR surf, VkExtent2D width_height, VkPresentModeKHR present_mode, unsigned swapchain_length);
	renderer(VkInstance ins, VkPhysicalDevice pd, VkSurfaceKHR surf0, VkSurfaceKHR surf1, VkExtent2D width_height, VkPresentModeKHR present_mode, unsigned swapchain_length);
	~renderer();

	void run(unsigned draw_calls_count = 1);

	void wait_idle();

protected:

	vk::render_pass create_render_pass(VkFormat color_format);

	void record(VkCommandBuffer cmdb, VkFramebuffer fb, float t, unsigned draw_calls_count);

	unsigned width;
	unsigned height;

	vk::device dev;

	VkQueue q0;

	vk::render_pass rp;

	triangle_renderer tr;

	struct output
	{
		vk::swapchain swp;

		std::vector<vk::image_view> swapchain_views;
		std::vector<vk::framebuffer> fbos;

		vk::semaphore image_acquired_semaphore;

		uint32_t image_index = 0;

		uint32_t acquire(VkDevice dev)
		{
			VkResult err = vkAcquireNextImageKHR(dev, swp, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &image_index);
			if (err != VK_SUCCESS)
				throw std::runtime_error("vkAcquireNextImageKHR failed");

			return image_index;
		}

		VkFramebuffer fbo() const { return fbos[image_index]; }
	};

	std::vector<output> outputs;

	renderer::output create_swapchain(VkSurfaceKHR surface, VkSurfaceFormatKHR swapchain_format, VkExtent2D width_height,
		VkPresentModeKHR present_mode, unsigned swapchain_length, VkSurfaceTransformFlagBitsKHR transform, VkBool32 present_barrier_enable);
	
			
	vk::semaphore draw_completed_semaphore;
	
	std::vector<vk::command_pool> cmdb_pools;
	std::vector<vk::command_buffer> draw_frame_cmdb;
	std::vector<vk::fence>	draw_completed_fences;
	int cmdb_index = 0;

	ig::high_resolution_timer timer;

private:

	std::vector<VkSemaphore> submit_wait_semaphores;
	std::vector<VkPipelineStageFlags> submit_wait_stages;

	std::vector<VkSwapchainKHR> present_swapchains;
	std::vector<uint32_t> present_image_indices;
};
