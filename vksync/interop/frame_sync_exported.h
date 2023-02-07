#pragma once

#include <GL/glew.h>

#include <igx/gl/semaphore.h>
#include <igx/vk/semaphore.h>
#include <igx/vk/semaphore_handle.h>
#include <igx/vk/fence.h>
#include <igx/vk/command_pool.h>
#include <igx/vk/command_buffer.h>


namespace interop
{

	class frame_sync_exported
	{
	public:

		frame_sync_exported() = default;

		frame_sync_exported(VkDevice dev, GLuint tex);

		frame_sync_exported(const frame_sync_exported&) = delete;
		frame_sync_exported& operator = (const frame_sync_exported&) = delete;

		frame_sync_exported(frame_sync_exported&& other) noexcept;
		frame_sync_exported& operator = (frame_sync_exported&& other) noexcept;

		void frame_begin(); 
		void frame_end();


		VkSemaphore acquired_semaphore() const { return image_acquired_semaphore; }

		VkSemaphore released_semaphore() const { return draw_completed_semaphore; }

	protected:

		vk::semaphore image_acquired_semaphore;
		vk::semaphore draw_completed_semaphore;

		GLuint gl_swp = 0;

		gl::semaphore gl_finished;
		gl::semaphore gl_acquire;

	};
}