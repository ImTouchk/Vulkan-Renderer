#include "..\Include\Definitions.h"

VkQueue g_GraphicsQueue;
VkDevice g_Device;

void CreateLogicalDevice() {
	if (g_ShouldTerminate) {
		return;
	}

	QueueFamilyIndices Indices = FindQueueFamilies(g_PhysicalDevice);

	VkDeviceQueueCreateInfo QueueCreateInfo{};
	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.queueFamilyIndex = Indices.graphicsFamily.value();
	QueueCreateInfo.queueCount = 1;

	float QueuePriority = 1.0f;
	QueueCreateInfo.pQueuePriorities = &QueuePriority;

	VkPhysicalDeviceFeatures DeviceFeatures{};

	VkDeviceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.pQueueCreateInfos = &QueueCreateInfo;
	CreateInfo.pEnabledFeatures = &DeviceFeatures;
	CreateInfo.enabledExtensionCount = 0;
	CreateInfo.queueCreateInfoCount = 1;
	CreateInfo.enabledLayerCount = 0;

	if (g_UseValidationLayers) {
		CreateInfo.enabledLayerCount = static_cast<unsigned int>(g_ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = g_ValidationLayers.data();
	}

	if (vkCreateDevice(g_PhysicalDevice, &CreateInfo, nullptr, &g_Device) != VK_SUCCESS) {
		std::cout << "[ERROR] Failed to create logical device." << std::endl;
		g_ShouldTerminate = true;
	} else {
		std::cout << "[INFO] Created logical device." << std::endl;
	}

	vkGetDeviceQueue(g_Device, Indices.graphicsFamily.value(), 0, &g_GraphicsQueue);
}

void DestroyLogicalDevice() {
	vkDestroyDevice(g_Device, nullptr);
}