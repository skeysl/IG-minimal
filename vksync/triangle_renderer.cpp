#include "triangle_renderer.h"

#include <igx/vk/shader_module.h>

#include <utility>


const uint32_t triangle_vert_sprv[] = 
{
#include "triangle.vert.sprv"
};

const uint32_t trianglel_frag_sprv[] = 
{
#include "triangle.frag.sprv"
};


triangle_renderer::triangle_renderer(VkDevice dev, VkRenderPass rp, unsigned width, unsigned height)
{
	VkPushConstantRange t_constant = { VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) };

	VkPipelineLayoutCreateInfo pl_create_info{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	pl_create_info.pushConstantRangeCount = 1;
	pl_create_info.pPushConstantRanges = &t_constant;

	triangle_pipeline_layout = vk::pipeline_layout(dev, &pl_create_info);

	vk::shader_module triangle_vs(dev, sizeof(triangle_vert_sprv), (const uint32_t *)triangle_vert_sprv);
	vk::shader_module triangle_fs(dev, sizeof(trianglel_frag_sprv), (const uint32_t *)trianglel_frag_sprv);
	

	VkPipelineShaderStageCreateInfo shaders[] = {
		{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_VERTEX_BIT, triangle_vs, "main", nullptr},
		{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FRAGMENT_BIT, triangle_fs, "main", nullptr}
	};

	VkPipelineVertexInputStateCreateInfo vi{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

	VkPipelineInputAssemblyStateCreateInfo ia{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
	ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		
	VkPipelineDynamicStateCreateInfo dyns{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
	
	int x = 0;
	int y = 0;

	VkViewport viewport;
	viewport.x = (float)x;
	viewport.y = (float)(height + y);
	viewport.width = (float)width;
	viewport.height = -(float)height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset.x = x;
	scissor.offset.y = y;
	scissor.extent.width = width;
	scissor.extent.height = height;

	VkPipelineViewportStateCreateInfo vp{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	vp.viewportCount = 1;
	vp.pViewports = &viewport;
	vp.scissorCount = 1;
	vp.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo cull_none_rs{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
	cull_none_rs.cullMode = VK_CULL_MODE_NONE;
	cull_none_rs.lineWidth = 1.0f;

	VkPipelineDepthStencilStateCreateInfo depth_stencil{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	depth_stencil.depthCompareOp = VK_COMPARE_OP_ALWAYS;
	
	VkPipelineMultisampleStateCreateInfo ms {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
	ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


	VkPipelineColorBlendAttachmentState blend_disable[] = {
		{ VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT }
	};

	VkPipelineColorBlendStateCreateInfo  blend{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
	blend.attachmentCount = 1;
	blend.pAttachments = blend_disable;
	blend.blendConstants[0] = 0.0f;
	blend.blendConstants[1] = 0.0f;
	blend.blendConstants[2] = 0.0f;
	blend.blendConstants[3] = 0.0f;

	VkGraphicsPipelineCreateInfo ci{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
	ci.stageCount = 2;
	ci.pStages = shaders;
	ci.pVertexInputState = &vi;
	ci.pInputAssemblyState = &ia;
	ci.pRasterizationState = &cull_none_rs;
	ci.pMultisampleState = &ms;
	ci.pDepthStencilState = &depth_stencil;
	ci.pColorBlendState = &blend;
	ci.pDynamicState = &dyns;
	ci.pViewportState = &vp;
	ci.layout = triangle_pipeline_layout;
	ci.renderPass = rp;
	ci.subpass = 0;

	triangle_pipeline = vk::pipeline(dev, VK_NULL_HANDLE, &ci);
}


triangle_renderer& triangle_renderer::operator = (triangle_renderer&& other)
{
	std::swap(triangle_pipeline_layout, other.triangle_pipeline_layout);
	std::swap(triangle_pipeline, other.triangle_pipeline);

	return *this;
}


void triangle_renderer::record(VkCommandBuffer cmdb, float t)
	{
		vkCmdBindPipeline(cmdb, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_pipeline);
		vkCmdPushConstants(cmdb, triangle_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(t), &t);
		vkCmdDraw(cmdb, 3, 1, 0, 0);
	}

