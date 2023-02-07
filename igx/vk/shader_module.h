#pragma once

#include "../igx_export.h"

#include <vulkan/vulkan.h>


namespace vk
{
	class IGX_DLL shader_module
	{
	public:

		shader_module() = default;
		shader_module(VkDevice d, const VkShaderModuleCreateInfo* create_info);
		shader_module(VkDevice d, size_t size, const uint32_t* code);

		template <class Source>
		shader_module(VkDevice d, const Source & source) : 
			shader_module(d, sizeof(source), (const uint32_t *)source)
		{
		}

		~shader_module();

		shader_module(shader_module&& other) noexcept;
		shader_module& operator = (shader_module&& other) noexcept;

		shader_module(const shader_module &) = delete;
		shader_module & operator = (const shader_module &) = delete;

		operator const VkShaderModule & () const { return sm; }
	
	protected:

		VkShaderModule sm = VK_NULL_HANDLE;
		VkDevice dev = VK_NULL_HANDLE;
	};
}
