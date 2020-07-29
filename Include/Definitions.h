#ifndef VARIABLES_H
#define VARIABLES_H

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <optional>
#include <vector>

//
extern VkDebugUtilsMessengerEXT g_DebugMessenger;
extern VkInstance g_Instance;

extern bool g_InstanceCreated;

//
extern VkPhysicalDevice g_PhysicalDevice;

struct QueueFamilyIndices {
	std::optional<unsigned int> graphicsFamily;
};

//
extern VkDevice g_Device;

//
extern bool g_ShouldTerminate;

//
extern GLFWwindow* g_Window;

extern int g_WinHeight;
extern int g_WinWidth;

//
void CreateInstance();
void DestroyInstance();

void SetupDebugMessenger();
void DestroyDebugMessenger();

void PickPhysicalDevice();

#endif