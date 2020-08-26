#include "..\Include\Definitions.h"

// External variable declarations
VkSwapchainKHR g_Swapchain;
VkExtent2D g_SwapchainExtent;
VkFormat g_SwapchainFormat;

// Internal function declarations
void CleanupSwapchain();

// External functions
void CreateSwapchain() {
	SwapchainSupportDetails SwapchainSupport = QuerySwapchainSupport(g_PhysicalDevice);
	
	VkSurfaceFormatKHR SurfaceFormat = PickSwapSurfaceFormat(SwapchainSupport.formats);
	VkPresentModeKHR PresentMode = PickSwapPresentMode(SwapchainSupport.presentModes);
	VkExtent2D Extent = PickSwapExtent(SwapchainSupport.capabilities);

	unsigned int ImageCount = SwapchainSupport.capabilities.minImageCount + 1;

	if (SwapchainSupport.capabilities.maxImageCount > 0 && ImageCount > SwapchainSupport.capabilities.maxImageCount) {
		ImageCount = SwapchainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.surface = g_Surface;
	CreateInfo.minImageCount = ImageCount;
	CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	CreateInfo.imageExtent = Extent;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices Indices = FindQueueFamilies(g_PhysicalDevice);
	unsigned int QueueFamilyIndices[] = { Indices.graphicsFamily.value(), Indices.presentFamily.value() };

	if (Indices.graphicsFamily != Indices.presentFamily) {
		CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
		CreateInfo.queueFamilyIndexCount = 2;
	} else {
		CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		CreateInfo.pQueueFamilyIndices = nullptr;
		CreateInfo.queueFamilyIndexCount = 0;
	}

	CreateInfo.preTransform = SwapchainSupport.capabilities.currentTransform;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.presentMode = PresentMode;
	CreateInfo.clipped = VK_TRUE;
	CreateInfo.oldSwapchain = VK_NULL_HANDLE;
	CreateInfo.imageFormat = SurfaceFormat.format;

	if (vkCreateSwapchainKHR(g_Device, &CreateInfo, nullptr, &g_Swapchain) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create swap chain." << std::endl;
	}

	vkGetSwapchainImagesKHR(g_Device, g_Swapchain, &ImageCount, nullptr);
	g_SwapchainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(g_Device, g_Swapchain, &ImageCount, g_SwapchainImages.data());

	g_SwapchainFormat = SurfaceFormat.format;
	g_SwapchainExtent = Extent;
}

void DestroySwapchain() {
	CleanupSwapchain();
}

void RecreateSwapchain() {
	int Width = 0, Height = 0;
	glfwGetFramebufferSize(g_Window, &Width, &Height);

	while (Width == 0 || Height == 0) {
		glfwGetFramebufferSize(g_Window, &Width, &Height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(g_Device);

	CleanupSwapchain();

	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateCommandBuffers();
}

// Internal functions
VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& Formats) {
	for (unsigned int i = 0; i < Formats.size(); i++) {
		if (Formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && Formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
			return Formats[i];
		}
	}
	return Formats[0];
}

VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& PresentModes) {
	for (unsigned int i = 0; i < PresentModes.size(); i++) {
		if (PresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			return PresentModes[i];
		}
	}
	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities) {
	if (Capabilities.currentExtent.width != UINT32_MAX) {
		return Capabilities.currentExtent;
	}

	VkExtent2D ActualExtent = { g_WinWidth, g_WinHeight };
	ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
	ActualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));
	return ActualExtent;
}

void CleanupSwapchain() {
	for (unsigned int i = 0; i < g_SwapchainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(g_Device, g_SwapchainFramebuffers[i], nullptr);
	}

	vkDestroyDescriptorPool(g_Device, g_DescriptorPool, nullptr);

	vkFreeCommandBuffers(g_Device, g_CommandPool, static_cast<unsigned int>(g_CommandBuffers.size()), g_CommandBuffers.data());
	
	vkDestroyPipeline(g_Device, g_Pipeline, nullptr);
	vkDestroyPipelineLayout(g_Device, g_PipelineLayout, nullptr);
	vkDestroyRenderPass(g_Device, g_RenderPass, nullptr);

	for (size_t i = 0; i < g_SwapchainImageViews.size(); i++) {
		vkDestroyImageView(g_Device, g_SwapchainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(g_Device, g_Swapchain, nullptr);

	for (size_t i = 0; i < g_SwapchainImages.size(); i++) {
		vkDestroyBuffer(g_Device, g_UniformBuffers[i], nullptr);
		vkFreeMemory(g_Device, g_UniformBufferMemories[i], nullptr);
	}

}