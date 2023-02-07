#include "frame_sync_exported.h"

#include <utility>


namespace interop
{
    frame_sync_exported::frame_sync_exported(VkDevice dev, GLuint tex) :
		gl_swp(tex)
	{
		VkExportSemaphoreCreateInfo export_info{ VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO };
		export_info.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT;

		VkSemaphoreCreateInfo create_info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, &export_info };
		
		image_acquired_semaphore = vk::semaphore(dev, &create_info);
		draw_completed_semaphore = vk::semaphore(dev, &create_info);

		gl_finished = gl::semaphore(get_semaphore_handle(dev, image_acquired_semaphore));
		gl_acquire = gl::semaphore(get_semaphore_handle(dev, draw_completed_semaphore));
	}


	frame_sync_exported::frame_sync_exported(frame_sync_exported&& other) noexcept :
		image_acquired_semaphore(std::move(other.image_acquired_semaphore)),
		draw_completed_semaphore(std::move(other.draw_completed_semaphore)),
		gl_swp(other.gl_swp),
		gl_finished(std::move(other.gl_finished)),
		gl_acquire(std::move(other.gl_acquire))		
	{
	}

	frame_sync_exported& frame_sync_exported::operator = (frame_sync_exported&& other) noexcept
	{
		std::swap(image_acquired_semaphore, other.image_acquired_semaphore);
		std::swap(draw_completed_semaphore, other.draw_completed_semaphore);
		std::swap(gl_swp, other.gl_swp);
		std::swap(gl_finished, other.gl_finished);
		std::swap(gl_acquire, other.gl_acquire);

		return *this;
	}
	
	void frame_sync_exported::frame_begin() 
	{
		gl_finished.signal_texture_semaphore(gl_swp, GL_LAYOUT_COLOR_ATTACHMENT_EXT);
	}


	void frame_sync_exported::frame_end()
	{
		gl_acquire.wait_texture_semaphore(gl_swp, GL_LAYOUT_COLOR_ATTACHMENT_EXT);
	}
}