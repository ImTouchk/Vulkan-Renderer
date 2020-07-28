#ifndef VARIABLES_H
#define VARIABLES_H

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>

extern GLFWwindow* g_Window;

extern bool g_InstanceCreated;
extern VkInstance g_Instance;

extern int g_WinHeight;
extern int g_WinWidth;

void CreateInstance();
void DestroyInstance();

#endif