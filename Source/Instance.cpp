#include "..\Include\Definitions.h"

// External variable declarations
VkDebugUtilsMessengerEXT g_DebugMessenger;
VkInstance g_Instance;

bool g_UseValidationLayers;
bool g_InstanceCreated;

std::vector<const char*> g_ValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

std::vector<const char*> g_DeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Internal variable declarations
bool UseValidationLayers = true;

// Function declarations
std::vector<const char*> GetRequiredExtensions();
void CheckExtensionSupport();
bool CheckLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Tyepe, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData);
void PopulateMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);

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

	std::vector<const char*> Extensions = GetRequiredExtensions();

	CreateInfo.enabledExtensionCount = static_cast<unsigned int>(Extensions.size());
	CreateInfo.ppEnabledExtensionNames = Extensions.data();
	CreateInfo.enabledLayerCount = 0;

	VkDebugUtilsMessengerCreateInfoEXT DebugInfo;

	if (UseValidationLayers && CheckLayerSupport()) {
		CreateInfo.enabledLayerCount = static_cast<unsigned int>(g_ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = g_ValidationLayers.data();

		PopulateMessengerCreateInfo(DebugInfo);
		CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}

	VkResult Result = vkCreateInstance(&CreateInfo, nullptr, &g_Instance);

	if (Result != VK_SUCCESS) {
		std::cout << "[ERROR] Couldn't create a Vulkan instance." << std::endl;
		g_ShouldTerminate = true;
		g_InstanceCreated = false;
	} else {
		g_InstanceCreated = true;
	}
}

void DestroyInstance() {
	vkDestroyInstance(g_Instance, nullptr);
}

// Internal functions
std::vector<const char*> GetRequiredExtensions() {
	unsigned int GLFWExtensionCount = 0;
	const char** GLFWExtensions;

	GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

	std::vector<const char*> Extensions(GLFWExtensions, GLFWExtensions + GLFWExtensionCount);

	if (UseValidationLayers) {
		Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return Extensions;
}

void CheckExtensionSupport() {
	unsigned int ExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);

	std::vector<VkExtensionProperties> Extensions(ExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, Extensions.data());
}

bool CheckLayerSupport() {
	unsigned int LayerCount = 0;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

	std::vector<VkLayerProperties> Layers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, Layers.data());

	for (unsigned i = 0; i < g_ValidationLayers.size(); i++) {
		bool LayerFound = false;

		for (unsigned j = 0; j < Layers.size(); j++) {
			if (strcmp(g_ValidationLayers[i], Layers[j].layerName) == 0) {
				LayerFound = true;
				break;
			}
		}

		if (!LayerFound) {
			return false;
		}
	}

	return true;
}

// Debugger
void SetupDebugMessenger() {
	if (!UseValidationLayers) {
		return;
	}

	if (g_ShouldTerminate) {
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT CreateInfo;
	PopulateMessengerCreateInfo(CreateInfo);

	auto Function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugUtilsMessengerEXT");
	VkResult Result;

	if (Function != nullptr) {
		Result = Function(g_Instance, &CreateInfo, nullptr, &g_DebugMessenger);
	}
	else Result = VK_ERROR_EXTENSION_NOT_PRESENT;

	if (Result != VK_SUCCESS) {
		std::cout << "[ERROR] Could not setup debug messenger." << std::endl;
	}
}

void DestroyDebugMessenger() {
	if (!UseValidationLayers) {
		return;
	}

	auto Function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugUtilsMessengerEXT");
	if (Function != nullptr) {
		Function(g_Instance, g_DebugMessenger, nullptr);
	}
}

void PopulateMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo) {
	CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	CreateInfo.pfnUserCallback = DebugCallback;
}

// Debug callback
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData) {
	if (Severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		std::cerr << "[ERROR] VALIDATION LAYER: " << CallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}