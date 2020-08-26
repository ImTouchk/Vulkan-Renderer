#ifndef VARIABLES_H
#define VARIABLES_H

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define	GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <functional>
#include <iostream>
#include <optional>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <set>

//
extern VkDebugUtilsMessengerEXT g_DebugMessenger;
extern VkInstance g_Instance;

extern std::vector<const char*> g_DeviceExtensions;
extern std::vector<const char*> g_ValidationLayers;
extern bool g_UseValidationLayers;
extern bool g_InstanceCreated;

//
extern VkPhysicalDevice g_PhysicalDevice;

struct QueueFamilyIndices {
	std::optional<unsigned int> graphicsFamily;
	std::optional<unsigned int> presentFamily;

	inline bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device);

//
extern std::vector<VkSemaphore> g_RendersFinished;
extern std::vector<VkSemaphore> g_ImagesAvailable;
extern std::vector<VkFence> g_InFlightImages;
extern std::vector<VkFence> g_InFlightFences;

//
extern std::vector<VkDeviceMemory> g_UniformBufferMemories;
extern std::vector<VkBuffer> g_UniformBuffers;

//
extern std::vector<VkCommandBuffer> g_CommandBuffers;
extern VkCommandPool g_CommandPool;

//
extern VkQueue g_GraphicsQueue;
extern VkQueue g_PresentQueue;
extern VkDevice g_Device;

//
extern std::vector<VkDescriptorSet> g_DescriptorSets;
extern VkDescriptorSetLayout g_DescriptorLayout;
extern VkDescriptorPool g_DescriptorPool;
extern VkPipelineLayout g_PipelineLayout;
extern VkRenderPass g_RenderPass;
extern VkPipeline g_Pipeline;

//
extern std::vector<VkFramebuffer> g_SwapchainFramebuffers;
extern std::vector<VkImageView> g_SwapchainImageViews;
extern std::vector<VkImage> g_SwapchainImages;
extern VkSwapchainKHR g_Swapchain;
extern VkExtent2D g_SwapchainExtent;
extern VkFormat g_SwapchainFormat;

extern int g_MaxFramesInFlight;
extern int g_CurrentFrame;

//
extern VkDeviceMemory g_DepthMemory;
extern VkImageView g_DepthView;
extern VkImage g_DepthImage;

//
extern VkDeviceMemory g_VertexBufferMemory;
extern VkDeviceMemory g_IndexBufferMemory;
extern VkBuffer g_VertexBuffer;
extern VkBuffer g_IndexBuffer;


extern VkSurfaceKHR g_Surface;

struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription BindingDescription{};
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		BindingDescription.stride = sizeof(Vertex);
		BindingDescription.binding = 0;
		return BindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> AttributeDescriptions{};
		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[0].offset = offsetof(Vertex, position);

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[1].offset = offsetof(Vertex, color);
		return AttributeDescriptions;
	}
};

struct UniformBufferObject {
	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
};

extern std::vector<uint16_t> Indices;
extern std::vector<Vertex> Vertices;

//
extern bool g_ShouldTerminate;

//
extern GLFWwindow* g_Window;

extern bool g_WinResized;
extern int g_WinHeight;
extern int g_WinWidth;

//
void CreateInstance();
void DestroyInstance();

void SetupDebugMessenger();
void DestroyDebugMessenger();

void CreateSurface();
void DestroySurface();

void CreateSwapchain();
void DestroySwapchain();

void RecreateSwapchain();

void CreateImageViews();
void DestroyImageViews();

void CreateGraphicsPipeline();
void DestroyGraphicsPipeline();

void CreateRenderPass();
void DestroyRenderPass();

void CreateFramebuffers();
void DestroyFramebuffers();

void CreateCommandPool();
void DestroyCommandPool();

void CreateCommandBuffers();
void DestroyCommandBuffers();

void CreateSemaphores();
void DestroySemaphores();

void CreateVertexBuffer();
void DestroyVertexBuffer();

void CreateIndexBuffer();
void DestroyIndexBuffer();

void CreateUniformBuffers();
void DestroyUniformBuffers();

void CreateDescriptorSetLayout();
void DestroyDescriptorSetLayout();

void CreateDescriptorPool();
void DestroyDescriptorPool();

void CreateDescriptorSets();

void CreateDepthResources();

void CreateBuffer(VkDeviceSize Size, VkBufferUsageFlags Flags, VkMemoryPropertyFlags PropertyFlags, VkBuffer& Buffer, VkDeviceMemory& Memory);
void CopyBuffer(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size);

std::vector<char> ReadFile(const std::string& Name);
VkShaderModule CreateShaderModule(const std::vector<char>& Code);

VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& Formats);
VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& PresentModes);
VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice Device);

void PickPhysicalDevice();

void CreateLogicalDevice();
void DestroyLogicalDevice();

#endif