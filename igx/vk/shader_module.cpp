#include "shader_module.h"

#include <stdexcept>


namespace vk
{
	shader_module::shader_module(VkDevice d, const VkShaderModuleCreateInfo * create_info) : dev(d)
	{
		VkResult err = vkCreateShaderModule(dev, create_info, NULL, &sm);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateShaderModule failed");
	}

	shader_module::shader_module(VkDevice d, size_t size, const uint32_t* code) : dev(d)
	{
		VkShaderModuleCreateInfo create_info{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		create_info.codeSize = size;
		create_info.pCode = code;

		VkResult err = vkCreateShaderModule(dev, &create_info, NULL, &sm);
		if (err != VK_SUCCESS)
			throw std::runtime_error("vkCreateShaderModule failed");
	}

		
	shader_module::~shader_module()
	{
		if (sm)
			vkDestroyShaderModule(dev, sm, NULL);
	}


	shader_module::shader_module(shader_module&& other) noexcept :
		sm(other.sm), 
		dev(other.dev)
	{
		other.sm = VK_NULL_HANDLE;
	}


	shader_module & shader_module::operator = (shader_module&& other) noexcept
	{
		std::swap(sm, other.sm);
		std::swap(dev, other.dev);
		return *this;
	}
}