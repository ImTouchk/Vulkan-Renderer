#include "..\Include\Definitions.h"

// External variable declarations
VkSurfaceKHR g_Surface;

// External functions
void CreateSurface() {
	if (g_ShouldTerminate) {
		return;
	}

	if (glfwCreateWindowSurface(g_Instance, g_Window, nullptr, &g_Surface) != VK_SUCCESS) {
		std::cout << "[ERROR] Could not create the window surface." << std::endl;
		g_ShouldTerminate = true;
	}
}

void DestroySurface() {
	vkDestroySurfaceKHR(g_Instance, g_Surface, nullptr);
}