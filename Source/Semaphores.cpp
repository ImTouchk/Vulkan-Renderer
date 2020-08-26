#include "..\Include\Definitions.h"

// External variable declarations
std::vector<VkSemaphore> g_RendersFinished;
std::vector<VkSemaphore> g_ImagesAvailable;
std::vector<VkFence> g_InFlightFences;
std::vector<VkFence> g_InFlightImages;

// External functions
void CreateSemaphores() {
	VkSemaphoreCreateInfo SemaphoreCreateInfo{};
	SemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo FenceCreateInfo{};
	FenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	g_InFlightImages.resize(g_MaxFramesInFlight, VK_NULL_HANDLE);
	g_InFlightFences.resize(g_MaxFramesInFlight);

	g_ImagesAvailable.resize(g_MaxFramesInFlight);
	g_RendersFinished.resize(g_MaxFramesInFlight);

	for (unsigned int i = 0; i < g_MaxFramesInFlight; i++) {
		if (vkCreateSemaphore(g_Device, &SemaphoreCreateInfo, nullptr, &g_ImagesAvailable[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Couldn't create image available semaphore." << std::endl;
			return;
		}

		if (vkCreateSemaphore(g_Device, &SemaphoreCreateInfo, nullptr, &g_RendersFinished[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Couldn't create render finished semaphore." << std::endl;
			return;
		}

		if (vkCreateFence(g_Device, &FenceCreateInfo, nullptr, &g_InFlightFences[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Couldn't create fence." << std::endl;
		}
	}
}

void DestroySemaphores() {
	for (unsigned int i = 0; i < g_MaxFramesInFlight; i++) {
		vkDestroySemaphore(g_Device, g_RendersFinished[i], nullptr);
		vkDestroySemaphore(g_Device, g_ImagesAvailable[i], nullptr);
		vkDestroyFence(g_Device, g_InFlightFences[i], nullptr);
	}
}