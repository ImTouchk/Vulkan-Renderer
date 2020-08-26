#include "..\Include\Definitions.h"

// External variable declarations
std::vector<VkImageView> g_SwapchainImageViews;
std::vector<VkImage> g_SwapchainImages;

// External functions
void CreateImageViews() {
	g_SwapchainImageViews.resize(g_SwapchainImages.size());

	for (unsigned int i = 0; i < g_SwapchainImages.size(); i++) {
		VkImageViewCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.subresourceRange.baseArrayLayer = 0;
		CreateInfo.subresourceRange.baseMipLevel = 0;
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.subresourceRange.levelCount = 1;
		CreateInfo.subresourceRange.layerCount = 1;
		CreateInfo.image = g_SwapchainImages[i];
		CreateInfo.format = g_SwapchainFormat;

		if (vkCreateImageView(g_Device, &CreateInfo, nullptr, &g_SwapchainImageViews[i]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Failed to create image view." << std::endl;
			return;
		}
	}
}

void DestroyImageViews() {
	for (unsigned int i = 0; i < g_SwapchainImageViews.size(); i++) {
		vkDestroyImageView(g_Device, g_SwapchainImageViews[i], nullptr);
	}
}