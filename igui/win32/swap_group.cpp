#ifdef WIN32
#include "swap_group.h"

#include <GL/wglew.h>

#include <stdexcept>
#include <iostream>


namespace igui::win32
{
	swap_group::swap_group(window& wnd, render_context& rc, int swp_group, int swp_barrier) :
		dc(wnd.device_context())
	{
		if (WGLEW_NV_swap_group)
		{
			GLuint  max_groups, max_barriers;

			if (!wglQueryMaxSwapGroupsNV(dc, &max_groups, &max_barriers))
				throw std::runtime_error("wglQueryMaxSwapGroupsNV failed!");

			std::cout << "max_groups: " << max_groups << " max_barriers: " << max_barriers << std::endl;

			GLuint current_group;
			GLuint current_barrier;
			if (!wglQuerySwapGroupNV(dc, &current_group, &current_barrier))
				throw std::runtime_error("wglQuerySwapGroupNV failed!");

			std::cout << "current group: " << current_group << " barrier:" << current_barrier << std::endl;

			if (swp_group != -1)
			{
				if (wglJoinSwapGroupNV(dc, swp_group) == GL_FALSE)
					throw std::runtime_error("wglJoinSwapGroupNV failed!");

				if (swp_barrier != -1)
					if (wglBindSwapBarrierNV(swp_group, swp_barrier) == GL_FALSE)
						throw std::runtime_error("wglBindSwapBarrierNV failed!");

				if (!wglQuerySwapGroupNV(dc, &current_group, &current_barrier))
					throw std::runtime_error("wglQuerySwapGroupNV failed!");

				std::cout << "joined group: " << current_group << " barrier:" << current_barrier << std::endl;
			}
		}
		else
			throw std::runtime_error("WGLEW_NV_swap_group extension is not present!");
	}

	swap_group::~swap_group()
	{
		//if (swp_barrier != -1)
		//	if (wglBindSwapBarrierNV(swp_group, 0) == GL_FALSE)
		//		IGLOG_ERROR("wglBindSwapBarrierNV failed!");

		//if (wglJoinSwapGroupNV(dc, 0) == GL_FALSE)
		//	IGLOG_ERROR("wglJoinSwapGroupNV failed!");
	}

	swap_group::swap_group(swap_group&& other) noexcept :
		dc(other.dc)
	{
		other.dc = NULL;
	}

	swap_group& swap_group ::operator = (swap_group&& other) noexcept
	{
		std::swap(dc, other.dc);

		return *this;
	}
}

#endif