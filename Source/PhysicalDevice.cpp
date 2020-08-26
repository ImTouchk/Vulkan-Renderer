#include "..\Include\Definitions.h"

// External variable declarations
VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;

// Function declarations
bool AreExtensionsSupported(VkPhysicalDevice Device);
bool IsDeviceSuitable(VkPhysicalDevice Device);

// External functions
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
	}
}

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device) {
	QueueFamilyIndices Indices;

	unsigned int FamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, nullptr);

	VkQueueFamilyProperties* QueueFamilies = new VkQueueFamilyProperties[FamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, QueueFamilies);

	for (unsigned int i = 0; i < FamilyCount; i++) {
		VkBool32 PresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, g_Surface, &PresentSupport);

		if (PresentSupport) {
			Indices.presentFamily = i;
		}

		if (QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			Indices.graphicsFamily = i;
		}

		if (Indices.isComplete()) {
			break;
		}
	}

	delete[] QueueFamilies;
	return Indices;
}

// Internal functions
bool IsDeviceSuitable(VkPhysicalDevice Device) {
	QueueFamilyIndices Indices = FindQueueFamilies(Device);

	bool SwapchainAdequate = false;
	if (AreExtensionsSupported(Device)) {
		SwapchainSupportDetails Details = QuerySwapchainSupport(Device);
		SwapchainAdequate = !Details.formats.empty() && !Details.presentModes.empty();
	}

	return Indices.isComplete() && AreExtensionsSupported(Device) && SwapchainAdequate;
}

bool AreExtensionsSupported(VkPhysicalDevice Device) {
	unsigned int ExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);

	VkExtensionProperties* AvailableExtensions = new VkExtensionProperties[ExtensionCount];
	vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions);

	std::set<std::string> RequiredExtensions(g_DeviceExtensions.begin(), g_DeviceExtensions.end());

	for (unsigned int i = 0; i < ExtensionCount; i++) {
		RequiredExtensions.erase(AvailableExtensions[i].extensionName);
	}

	delete[] AvailableExtensions;
	return RequiredExtensions.empty();
}

SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice Device) {
	SwapchainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, g_Surface, &Details.capabilities);

	unsigned int FormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device, g_Surface, &FormatCount, nullptr);

	if (FormatCount != 0) {
		Details.formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, g_Surface, &FormatCount, Details.formats.data());
	}

	unsigned int PresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device, g_Surface, &PresentModeCount, nullptr);
	
	if (PresentModeCount != 0) {
		Details.presentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, g_Surface, &PresentModeCount, Details.presentModes.data());
	}
	return Details;
}