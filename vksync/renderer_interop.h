#pragma once

#include <vulkan/vulkan.h>

#include <igx/vk/device.h>
#include <igx/vk/swapchain.h>
#include <igx/vk/image.h>
#include <igx/vk/image_view.h>
#include <igx/vk/render_pass.h>
#include <igx/vk/framebuffer.h>
#include <igx/vk/semaphore.h>
#include <igx/vk/fence.h>
#include <igx/vk/command_pool.h>
#include <igx/vk/command_buffer.h>
#include <igx/vk/device_memory.h>

#include <igx/gl/framebuffer.h>
#include <igx/gl/texture.h>
#include <igx/gl/memory_object.h>

#include "interop/frame_sync_exported.h"

#include "triangle_renderer.h"

#include <ig/timer.h>

#include <Windows.h>


class renderer_interop
{
public:

	renderer_interop(VkInstance ins, VkPhysicalDevice pd, unsigned w, unsigned h, unsigned swapchain_length);
	~renderer_interop();

	void run(unsigned draw_calls_count = 1);

	void wait_idle();

protected:

	void create_render_pass(VkFormat color_format);
	void create_framebuffer(int w, int h);

	void record(VkCommandBuffer cmdb, VkFramebuffer fb, float t, unsigned draw_calls_count);

	unsigned width;
	unsigned height;
		
	unsigned frame_index = 0;


	vk::device dev;

	VkQueue q0;

	vk::image swp_img;
	vk::device_memory swp_img_mem;
	vk::image_view swp_imgv;
	gl::memory_object swp_mem_gl;
	gl::texture_2d swp_tex_gl;
	gl::framebuffer swp_fbo_gl;
	
		
	vk::render_pass rp;
	vk::framebuffer output;

	triangle_renderer tr;	
	
	interop::frame_sync_exported frame;
		
	std::vector<vk::command_pool> cmdb_pools;
	std::vector<vk::command_buffer> draw_frame_cmdb;
	std::vector<vk::fence>	draw_completed_fences;
	
	int cmdb_index = 0;

	ig::high_resolution_timer timer;
};
