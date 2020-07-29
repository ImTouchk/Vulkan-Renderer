#include "..\Include\Definitions.h"

VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;

bool IsDeviceSuitable(VkPhysicalDevice Device);

void PickPhysicalDevice() {
	if (g_ShouldTerminate) {
		return;
	}
	
	unsigned int DeviceCount = 0;
	vkEnumeratePhysicalDevices(g_Instance, &DeviceCount, nullptr);

	std::vector<VkPhysicalDevice> Devices(DeviceCount);
	vkEnumeratePhysicalDevices(g_Instance, &DeviceCount, Devices.data());

	for (unsigned int i = 0; i < Devices.size(); i++) {
		if (IsDeviceSuitable(Devices[i])) {
			g_PhysicalDevice = Devices[i];
			break;
		}
	}

	if (g_PhysicalDevice == VK_NULL_HANDLE) {
		std::cout << "[ERROR] Failed to find a suitable GPU." << std::endl;
		g_ShouldTerminate = true;
		return;
	} else {
		std::cout << "[INFO] Succesfully picked a physical device." << std::endl;
	}
}

bool IsDeviceSuitable(VkPhysicalDevice Device) {
	QueueFamilyIndices Indices = FindQueueFamilies(Device);
	return Indices.graphicsFamily.has_value();
}

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device) {
	QueueFamilyIndices Indices;

	unsigned int FamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, nullptr);
	
	VkQueueFamilyProperties* QueueFamilies = new VkQueueFamilyProperties[FamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, QueueFamilies);

	for (unsigned int i = 0; i < FamilyCount; i++) {
		if (QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			Indices.graphicsFamily = i;
		}

		if (Indices.graphicsFamily.has_value()) {
			break;
		}
	}

	delete[] QueueFamilies;
	return Indices;
}