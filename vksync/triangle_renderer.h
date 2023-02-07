#pragma once

#include <vulkan/vulkan.h>

#include <igx/vk/pipeline_layout.h>
#include <igx/vk/pipeline.h>


class triangle_renderer
{
public:

	triangle_renderer() = default;

	triangle_renderer(VkDevice d, VkRenderPass rp, unsigned width, unsigned height);
	triangle_renderer& operator = (triangle_renderer&& other);

	void record(VkCommandBuffer cmdb, float t);

public:

	vk::pipeline_layout triangle_pipeline_layout;
	vk::pipeline triangle_pipeline;
};


