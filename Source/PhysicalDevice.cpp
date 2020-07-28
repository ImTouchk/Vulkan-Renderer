#include "..\Include\Definitions.h"

VkPhysicalDevice g_PhysicalDevice;

bool IsDeviceSuitable();

void PickPhysicalDevice() {
	unsigned int DeviceCount = 0;
	vkEnumeratePhysicalDevices(g_Instance, &DeviceCount, nullptr);

	std::vector<VkPhysicalDevice> Devices(DeviceCount);
	vkEnumeratePhysicalDevices(g_Instance, &DeviceCount, Devices.data());
}

