#include "..\Include\Definitions.h"

bool g_InstanceCreated;
VkInstance g_Instance;

void CheckExtensionSupport();

// Exteral functions
void CreateInstance() {
	VkApplicationInfo ApplicationInfo{};
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pApplicationName = "Vulkan Test";
	ApplicationInfo.pEngineName = "Vulkan Test";
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &ApplicationInfo;

	unsigned int GLFWExtensionCount = 0;
	const char** GLFWExtensions;

	GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

	CreateInfo.enabledExtensionCount = GLFWExtensionCount;
	CreateInfo.ppEnabledExtensionNames = GLFWExtensions;
	CreateInfo.enabledLayerCount = 0;

	CheckExtensionSupport();

	VkResult Result = vkCreateInstance(&CreateInfo, nullptr, &g_Instance);

	if (Result != VK_SUCCESS) {
		std::cout << "Couldn't create a Vulkan instance." << std::endl;
		g_InstanceCreated = false;
	} else {
		std::cout << "Vulkan instance created." << std::endl;
		g_InstanceCreated = true;
	}
}

void DestroyInstance() {
	vkDestroyInstance(g_Instance, nullptr);
}

// Internal functions
void CheckExtensionSupport() {
	unsigned int ExtensionCount = 0;

	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);

	std::vector<VkExtensionProperties> Extensions(ExtensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, Extensions.data());

	std::cout << "Available extensions: ";

	for (unsigned int i = 0; i < Extensions.size(); i++) {
		if (i != (Extensions.size() - 1)) {
			std::cout << Extensions[i].extensionName << ", ";
		}
		else {
			std::cout << Extensions[i].extensionName << "." << std::endl;
		}
	}
}