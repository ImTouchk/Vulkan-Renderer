#include "..\Include\Definitions.h"

// External variable definitions
VkPipelineLayout g_PipelineLayout;
VkPipeline g_Pipeline;

// External functions
void CreateGraphicsPipeline() {
	std::vector<char> VertexCode = ReadFile("D:/Development/C++/Projects/Vulkan-Test/Shaders/vert.spv");
	std::vector<char> FragmentCode = ReadFile("D:/Development/C++/Projects/Vulkan-Test/Shaders/frag.spv");

	VkShaderModule VertexModule = CreateShaderModule(VertexCode);
	VkShaderModule FragmentModule = CreateShaderModule(FragmentCode);

	VkPipelineShaderStageCreateInfo VertexCreateInfo{};
	VertexCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	VertexCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	VertexCreateInfo.module = VertexModule;
	VertexCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FragmentCreateInfo{};
	FragmentCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FragmentCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FragmentCreateInfo.module = FragmentModule;
	FragmentCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexCreateInfo, FragmentCreateInfo };

	auto BindingDescription = Vertex::GetBindingDescription();
	auto AttributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo VertexInputStateInfo{};
	VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputStateInfo.vertexAttributeDescriptionCount = AttributeDescriptions.size();
	VertexInputStateInfo.pVertexAttributeDescriptions = AttributeDescriptions.data();
	VertexInputStateInfo.pVertexBindingDescriptions = &BindingDescription;
	VertexInputStateInfo.vertexBindingDescriptionCount = 1;

	VkPipelineInputAssemblyStateCreateInfo AssemblyStateCreateInfo{};
	AssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	AssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	AssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport{};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = (float)g_SwapchainExtent.width;
	Viewport.height = (float)g_SwapchainExtent.height;
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D Scissor{};
	Scissor.offset = { 0, 0 };
	Scissor.extent = g_SwapchainExtent;

	VkPipelineViewportStateCreateInfo ViewportStateCreateInfo{};
	ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportStateCreateInfo.pViewports = &Viewport;
	ViewportStateCreateInfo.pScissors = &Scissor;
	ViewportStateCreateInfo.viewportCount = 1;
	ViewportStateCreateInfo.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo RasterizerCreateInfo{};
	RasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL; // VK_POLYGON_MODE_LINE / VK_POLYGON_MODE_POINT / VK_POLYGON_MODE_FILL
	RasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	RasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	RasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
	RasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;
	RasterizerCreateInfo.depthClampEnable = VK_FALSE;
	RasterizerCreateInfo.depthBiasEnable = VK_FALSE;
	RasterizerCreateInfo.depthBiasClamp = 0.0f;
	RasterizerCreateInfo.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo MultisampleCreateInfo{};
	MultisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	MultisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	MultisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	MultisampleCreateInfo.alphaToOneEnable = VK_FALSE;
	MultisampleCreateInfo.minSampleShading = 1.0f;
	MultisampleCreateInfo.pSampleMask = nullptr;

	VkPipelineColorBlendAttachmentState ColorBlendState{};
	ColorBlendState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	ColorBlendState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	ColorBlendState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	ColorBlendState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	ColorBlendState.colorBlendOp = VK_BLEND_OP_ADD;
	ColorBlendState.alphaBlendOp = VK_BLEND_OP_ADD;
	ColorBlendState.blendEnable = VK_TRUE;

	VkPipelineColorBlendStateCreateInfo ColorBlendCreateInfo{};
	ColorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendCreateInfo.pAttachments = &ColorBlendState;
	ColorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	ColorBlendCreateInfo.blendConstants[0] = 0.0f;
	ColorBlendCreateInfo.blendConstants[1] = 0.0f;
	ColorBlendCreateInfo.blendConstants[2] = 0.0f;
	ColorBlendCreateInfo.blendConstants[3] = 0.0f;
	ColorBlendCreateInfo.logicOpEnable = VK_FALSE;
	ColorBlendCreateInfo.attachmentCount = 1;

	VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo{};
	PipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutCreateInfo.pSetLayouts = &g_DescriptorLayout;
	PipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
	PipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	PipelineLayoutCreateInfo.setLayoutCount = 1;

	if (vkCreatePipelineLayout(g_Device, &PipelineLayoutCreateInfo, nullptr, &g_PipelineLayout) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create pipeline layout." << std::endl;
		return;
	}

	VkGraphicsPipelineCreateInfo PipelineCreateInfo{};
	PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineCreateInfo.pInputAssemblyState = &AssemblyStateCreateInfo;
	PipelineCreateInfo.pRasterizationState = &RasterizerCreateInfo;
	PipelineCreateInfo.pMultisampleState = &MultisampleCreateInfo;
	PipelineCreateInfo.pVertexInputState = &VertexInputStateInfo;
	PipelineCreateInfo.pViewportState = &ViewportStateCreateInfo;
	PipelineCreateInfo.pColorBlendState = &ColorBlendCreateInfo;
	PipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	PipelineCreateInfo.pDepthStencilState = nullptr;
	PipelineCreateInfo.renderPass = g_RenderPass;
	PipelineCreateInfo.layout = g_PipelineLayout;
	PipelineCreateInfo.pDynamicState = nullptr;
	PipelineCreateInfo.pStages = ShaderStages;
	PipelineCreateInfo.basePipelineIndex = -1;
	PipelineCreateInfo.stageCount = 2;
	PipelineCreateInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(g_Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &g_Pipeline) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create pipeline." << std::endl;
	}

	vkDestroyShaderModule(g_Device, VertexModule, nullptr);
	vkDestroyShaderModule(g_Device, FragmentModule, nullptr);
}

void DestroyGraphicsPipeline() {
	vkDestroyPipeline(g_Device, g_Pipeline, nullptr);
	vkDestroyPipelineLayout(g_Device, g_PipelineLayout, nullptr);
}