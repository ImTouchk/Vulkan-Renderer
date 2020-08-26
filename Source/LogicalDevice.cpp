#include "..\Include\Definitions.h"

// External variable declarations
VkQueue g_GraphicsQueue;
VkQueue g_PresentQueue;
VkDevice g_Device;

// External functions
void CreateLogicalDevice() {
	if (g_ShouldTerminate) {
		return;
	}

	QueueFamilyIndices Indices = FindQueueFamilies(g_PhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
	std::set<unsigned int> UniqueQueueFamilies = { Indices.graphicsFamily.value(), Indices.presentFamily.value() };

	float QueuePriority = 1.0f;

	for (unsigned int i = 0; i < UniqueQueueFamilies.size(); i++) {
		VkDeviceQueueCreateInfo QueueCreateInfo{};
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.pQueuePriorities = &QueuePriority;
		QueueCreateInfo.queueFamilyIndex = i;
		QueueCreateInfo.queueCount = 1;

		QueueCreateInfos.push_back(QueueCreateInfo);
	}

	VkPhysicalDeviceFeatures DeviceFeatures{};

	VkDeviceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.pEnabledFeatures = &DeviceFeatures;
	CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	CreateInfo.queueCreateInfoCount = static_cast<unsigned int>(QueueCreateInfos.size());
	CreateInfo.enabledExtensionCount = static_cast<unsigned int>(g_DeviceExtensions.size());
	CreateInfo.ppEnabledExtensionNames = g_DeviceExtensions.data();
	CreateInfo.enabledLayerCount = 0;

	if (g_UseValidationLayers) {
		CreateInfo.enabledLayerCount = static_cast<unsigned int>(g_ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = g_ValidationLayers.data();
	}

	if (vkCreateDevice(g_PhysicalDevice, &CreateInfo, nullptr, &g_Device) != VK_SUCCESS) {
		std::cout << "[ERROR] Failed to create logical device." << std::endl;
		g_ShouldTerminate = true;
	}

	vkGetDeviceQueue(g_Device, Indices.graphicsFamily.value(), 0, &g_GraphicsQueue);
	vkGetDeviceQueue(g_Device, Indices.presentFamily.value(), 0, &g_PresentQueue);
}

void DestroyLogicalDevice() {
	vkDestroyDevice(g_Device, nullptr);
}