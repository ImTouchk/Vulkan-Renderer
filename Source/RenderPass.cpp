#include "..\Include\Definitions.h"

// External variable definitions
VkRenderPass g_RenderPass;

// External functions
void CreateRenderPass() {
	VkAttachmentDescription ColorAttachment{};
	ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	ColorAttachment.format = g_SwapchainFormat;

	VkAttachmentReference ColorAttachmentReference{};
	ColorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	ColorAttachmentReference.attachment = 0;

	VkSubpassDescription SubpassDescription{};
	SubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	SubpassDescription.pColorAttachments = &ColorAttachmentReference;
	SubpassDescription.colorAttachmentCount = 1;

	VkSubpassDependency Dependency{};
	Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	Dependency.srcAccessMask = 0;
	Dependency.dstSubpass = 0;

	VkRenderPassCreateInfo RenderPassCreateInfo{};
	RenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassCreateInfo.pSubpasses = &SubpassDescription;
	RenderPassCreateInfo.pAttachments = &ColorAttachment;
	RenderPassCreateInfo.pDependencies = &Dependency;
	RenderPassCreateInfo.attachmentCount = 1;
	RenderPassCreateInfo.dependencyCount = 1;
	RenderPassCreateInfo.subpassCount = 1;

	if (vkCreateRenderPass(g_Device, &RenderPassCreateInfo, nullptr, &g_RenderPass) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create render pass." << std::endl;
		return;
	}
}

void DestroyRenderPass() {
	vkDestroyRenderPass(g_Device, g_RenderPass, nullptr);
}