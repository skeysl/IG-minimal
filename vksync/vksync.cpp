#include <ig/log.h>

#include <igui/win32/application.h>
#include <igui/win32/window.h>
#include <igui/win32/render_context.h>
#include <igui/win32/swap_group.h>

#include <igx/vk/surface.h>

#include "vulkan_instance.h"

#include "renderer_vk.h"
#include "renderer_gl.h"
#include "renderer_interop.h"

#include <stdexcept>
#include <vector>
#include <thread>
#include <barrier>


bool debug = false;

unsigned width = 1920;//2560;
unsigned height = 1200;//1600;
unsigned swapchain_lenght = 3;
VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

unsigned draw_calls_count = 700;//3000;


int main_vk()
{
	try
	{
		igui::win32::application app;

		std::vector<const char*> instance_extensions;

		if (debug)
		{
			instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		instance_extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

		app.for_each_required_instance_extension([&](const char* required_extension)
			{
				IGLOG("ui required extension " << required_extension);
				instance_extensions.push_back(required_extension);
			});
				

		vulkan_instance inst(instance_extensions, debug);


		std::atomic<bool> running = true;

		auto renderer_fn = [&](unsigned monitor, unsigned gpu)
		{
			try
			{
				igui::win32::window wnd0(width, height, "gpu0", monitor);
				vk::surface surface0(inst.instance(), wnd0);

				renderer r0(inst.instance(), inst.physical_device(gpu), surface0, { width, height }, present_mode, swapchain_lenght);

				while (running)
				{
					if (!app.poll())
						running = false;

					r0.run(draw_calls_count);
				}

			}
			catch (std::exception& e)
			{
				IGLOG_ERROR(e.what());
			}
		};

		std::jthread r2_thread(renderer_fn, 0, 0);
		std::jthread r0_thread(renderer_fn, 1, 1);
		std::jthread r1_thread(renderer_fn, 2, 2);
	}
	catch (std::exception& e)
	{
		IGLOG_ERROR(e.what());
	}

	return 0;
}


int main_vk_multimonitor()
{
	try
	{
		igui::win32::application app;

		std::vector<const char*> instance_extensions;

		if (debug)
		{
			instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		instance_extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

		app.for_each_required_instance_extension([&](const char* required_extension)
			{
				IGLOG("ui required extension " << required_extension);
				instance_extensions.push_back(required_extension);
			});


		vulkan_instance inst(instance_extensions, debug);


		std::atomic<bool> running = true;

		auto renderer_fn = [&](unsigned monitor0, unsigned monitor1, unsigned gpu)
		{
			try
			{
				igui::win32::window wnd0(width, height, "gpu0", monitor0);
				igui::win32::window wnd1(width, height, "gpu0", monitor1);
				vk::surface surface0(inst.instance(), wnd0);
				vk::surface surface1(inst.instance(), wnd1);

				renderer r0(inst.instance(), inst.physical_device(gpu), surface0, surface1, { width, height }, present_mode, swapchain_lenght);

				while (running)
				{
					if (!app.poll())
						running = false;

					r0.run(draw_calls_count);
				}

			}
			catch (std::exception& e)
			{
				IGLOG_ERROR(e.what());
			}
		};

		std::jthread r2_thread(renderer_fn, 0, 1, 0);
		//std::jthread r0_thread(renderer_fn, 1, 1);
		//std::jthread r1_thread(renderer_fn, 2, 2);
	}
	catch (std::exception& e)
	{
		IGLOG_ERROR(e.what());
	}

	return 0;
}


int main_gl()
{
	try
	{
		igui::win32::application app;

		std::atomic<bool> running = true;

		auto render_fn = [&](int monitor, int affinity)
		{
			try
			{
				igui::win32::window wnd(width, height, "wnd",
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, 32,
				0
			},
				{
					WGL_CONTEXT_FLAGS_ARB, (debug) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
					WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
					WGL_CONTEXT_MINOR_VERSION_ARB, 6,
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0
				}, monitor, affinity);

				igui::win32::render_context rc(wnd);
				igui::win32::swap_group sg(wnd, rc, 1, 1);

				if ((debug) && (GLEW_ARB_debug_output))
				{
					glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
						{
							IGLOG(message);
						}, nullptr);
					glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				}

				renderer_gl r0;

				while (running)
				{
					if (!app.poll())
						running = false;

					auto [w, h] = wnd.size();
					r0.render(w, h, draw_calls_count);

					wnd.swap_buffers();
				}
			}
			catch (std::exception& e)
			{
				IGLOG_ERROR(e.what());
			}
		};

		
		std::jthread r0_thread(render_fn, 0, 0);
		std::jthread r1_thread(render_fn, 1, 1);
		std::jthread r2_thread(render_fn, 2, 2);
	}
	catch (std::exception& e)
	{
		IGLOG_ERROR(e.what());
	}

	return 0;
}


int main_gl_interop()
{
	try
	{
		igui::win32::application app;

		std::vector<const char*> instance_extensions;

		if (debug)
		{
			instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		vulkan_instance inst(instance_extensions, debug);

		std::atomic<bool> running = true;

		auto render_fn = [&](int monitor, int affinity, int gpu_index)
		{
			try
			{
				igui::win32::window wnd(width, height, "wnd",
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, 32,
				0
			},
				{
					WGL_CONTEXT_FLAGS_ARB, (debug) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
					WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
					WGL_CONTEXT_MINOR_VERSION_ARB, 6,
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0
				}, monitor, affinity);

				igui::win32::render_context rc(wnd);
				igui::win32::swap_group sg(wnd, rc, 1, 1);

				
				// interop extensions
				if ((!GLEW_EXT_memory_object) ||  
					(!GLEW_EXT_semaphore))
						throw std::runtime_error("GL_EXT_memory_object and GL_EXT_semaphore unsupported!");
				
				if ((debug) && (GLEW_ARB_debug_output))
				{
					glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
						{
							IGLOG(message);
						}, nullptr);
					glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				}

				renderer_interop r(inst.instance(), inst.physical_device(gpu_index), width, height, 3);

				while (running)
				{
					if (!app.poll())
						running = false;

					r.run(draw_calls_count);

					wnd.swap_buffers();
				}
			}
			catch (std::exception& e)
			{
				IGLOG_ERROR(e.what());
			}
		};

		
		std::jthread r0_thread(render_fn, 0, 0, 0);
		std::jthread r1_thread(render_fn, 1, 1, 1);
		std::jthread r2_thread(render_fn, 2, 2, 2);
	}
	catch (std::exception& e)
	{
		IGLOG_ERROR(e.what());
	}

	return 0;
}



int main()
{
	return main_vk();
//	return main_gl();
//	return main_gl_interop();

	//return main_vk_multimonitor();
}