#include "..\Include\Definitions.h"

// External variable declarations
VkCommandPool g_CommandPool;

// External functions
void CreateCommandPool() {
	QueueFamilyIndices FamilyIndices = FindQueueFamilies(g_PhysicalDevice);
	VkCommandPoolCreateInfo PoolCreateInfo{};
	PoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	PoolCreateInfo.queueFamilyIndex = FamilyIndices.graphicsFamily.value();
	PoolCreateInfo.flags = 0;

	if (vkCreateCommandPool(g_Device, &PoolCreateInfo, nullptr, &g_CommandPool) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create command pool." << std::endl;
		return;
	}
}

void DestroyCommandPool() {
	vkDestroyCommandPool(g_Device, g_CommandPool, nullptr);
}