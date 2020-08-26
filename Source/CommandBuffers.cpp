#include "..\Include\Definitions.h"

// External variable declarations
std::vector<VkCommandBuffer> g_CommandBuffers;

// External functions
void CreateCommandBuffers() {
	g_CommandBuffers.resize(g_SwapchainFramebuffers.size());
	VkCommandBufferAllocateInfo AllocateInfo{};
	AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocateInfo.commandBufferCount = g_CommandBuffers.size();
	AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocateInfo.commandPool = g_CommandPool;

	if (vkAllocateCommandBuffers(g_Device, &AllocateInfo, g_CommandBuffers.data()) != VK_SUCCESS) {
		std::cerr << "[ERROR] Couldn't allocate command buffers." << std::endl;
		return;
	}

	const VkClearValue ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	VkCommandBufferBeginInfo BeginInfo{};
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.pInheritanceInfo = nullptr;
	BeginInfo.flags = 0;

	for (unsigned int i = 0; i < g_CommandBuffers.size(); i++) {
		if (vkBeginCommandBuffer(g_CommandBuffers[i], &BeginInfo) != VK_SUCCESS) {
			std::cerr << "[ERROR] Couldn't begin command buffer." << std::endl;
			break;
		}

		VkRenderPassBeginInfo RenderPassInfo{};
		RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassInfo.framebuffer = g_SwapchainFramebuffers[i];
		RenderPassInfo.renderArea.extent = g_SwapchainExtent;
		RenderPassInfo.renderArea.offset = { 0, 0 };
		RenderPassInfo.pClearValues = &ClearColor;
		RenderPassInfo.renderPass = g_RenderPass;
		RenderPassInfo.clearValueCount = 1;
		
		VkBuffer VertexBuffers[] = { g_VertexBuffer };
		VkDeviceSize Offsets[] = { 0 };

		vkCmdBeginRenderPass(g_CommandBuffers[i], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(g_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, g_Pipeline);
		vkCmdBindVertexBuffers(g_CommandBuffers[i], 0, 1, VertexBuffers, Offsets);
		vkCmdBindIndexBuffer(g_CommandBuffers[i], g_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(g_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, g_PipelineLayout, 0, 1, &g_DescriptorSets[i], 0, nullptr);
		vkCmdDrawIndexed(g_CommandBuffers[i], Indices.size(), 1, 0, 0, 0);
		vkCmdEndRenderPass(g_CommandBuffers[i]);

		if (vkEndCommandBuffer(g_CommandBuffers[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Failed to record command buffer." << std::endl;
			break;
		}
	}

}

void DestroyCommandBuffers() {

}