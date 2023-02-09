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

#include <Windows.h>


class renderer
{
public:

	renderer(VkInstance ins, VkPhysicalDevice pd, VkSurfaceKHR surf, unsigned w, unsigned h, VkPresentModeKHR present_mode, unsigned swapchain_length);
	~renderer();

	void run(unsigned draw_calls_count = 1);

	void wait_idle();

protected:

	void create_swapchain(VkSurfaceKHR surface, VkSurfaceFormatKHR swapchain_format, unsigned w, unsigned h, 
		VkPresentModeKHR present_mode, unsigned swapchain_length, VkSurfaceTransformFlagBitsKHR transform, VkBool32 present_barrier_enable);
	void create_render_pass(VkFormat color_format);

	void record(VkCommandBuffer cmdb, VkFramebuffer fb, float t, unsigned draw_calls_count);

	unsigned width;
	unsigned height;

	VkSurfaceKHR surface;
	HMONITOR monitor;

	unsigned frame_index = 0;


	vk::device dev;

	VkQueue q0;

	vk::swapchain swp;
	vk::swapchain old_swapchain;
	
	std::vector<vk::image_view> swapchain_views;
			
	vk::render_pass rp;
	
	std::vector<vk::framebuffer> outputs;

	triangle_renderer tr;	
	
	vk::semaphore image_acquired_semaphore;
	vk::semaphore draw_completed_semaphore;
	
	std::vector<vk::command_pool> cmdb_pools;
	std::vector<vk::command_buffer> draw_frame_cmdb;
	std::vector<vk::fence>	draw_completed_fences;
	int cmdb_index = 0;

	ig::high_resolution_timer timer;

private:
	VkSurfaceFormatKHR swapchain_format;
	VkSurfaceTransformFlagBitsKHR transform;
};
