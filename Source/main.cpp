#include "..\Include\Definitions.h"

GLFWwindow* g_Window = nullptr;

int g_WinWidth = 800;
int g_WinHeight = 600;

bool g_ShouldTerminate = false;

void Initialize() {
	CreateInstance();
	SetupDebugMessenger();
	PickPhysicalDevice();
	CreateLogicalDevice();
}

void Destroy() {
	DestroyLogicalDevice();
	DestroyDebugMessenger();
	DestroyInstance();
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (!glfwVulkanSupported()) {
		std::cout << "This machine does not support Vulkan." << std::endl;
		glfwTerminate();
		return -1;
	}

	g_Window = glfwCreateWindow(g_WinWidth, g_WinHeight, "Hello", nullptr, nullptr);

	Initialize();

	while (!glfwWindowShouldClose(g_Window)) {
		glfwPollEvents();
	}

	Destroy();

	glfwDestroyWindow(g_Window);
	glfwTerminate();

	return 1;
}