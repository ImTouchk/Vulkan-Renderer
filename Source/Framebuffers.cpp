#include "..\Include\Definitions.h"

// External variable declarations
std::vector<VkFramebuffer> g_SwapchainFramebuffers;

// External functions
void CreateFramebuffers() {
	g_SwapchainFramebuffers.resize(g_SwapchainImageViews.size());
	
	for (unsigned int i = 0; i < g_SwapchainImageViews.size(); i++) {
		VkFramebufferCreateInfo FramebufferCreateInfo{};
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.pAttachments = &g_SwapchainImageViews[i];
		FramebufferCreateInfo.height = g_SwapchainExtent.height;
		FramebufferCreateInfo.width = g_SwapchainExtent.width;
		FramebufferCreateInfo.renderPass = g_RenderPass;
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(g_Device, &FramebufferCreateInfo, nullptr, &g_SwapchainFramebuffers[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Failed to create frame buffer." << std::endl;
			break;
		}
	}
}

void DestroyFramebuffers() {
	for (unsigned int i = 0; i < g_SwapchainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(g_Device, g_SwapchainFramebuffers[i], nullptr);
	}
}