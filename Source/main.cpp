#include "..\Include\Definitions.h"

GLFWwindow* g_Window = nullptr;
bool g_ShouldTerminate = false;
bool g_WinResized = false;

int g_MaxFramesInFlight = 2;
int g_CurrentFrame = 0;
int g_WinHeight = 600;
int g_WinWidth = 800;

double PreviousTime = 0.0f;
double CurrentTime = 0.0f;
int FPS = 0;

std::vector<Vertex> Vertices = {
	{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
};

std::vector<uint16_t> Indices = {
	0, 1, 2,
	2, 3, 0
};

void InitializeVulkan() {
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateCommandPool();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateCommandBuffers();
	CreateSemaphores();
}

void Destroy() {
	DestroySwapchain();
	DestroyDescriptorPool();
	DestroyDescriptorSetLayout();
	DestroyIndexBuffer();
	DestroyVertexBuffer();
	DestroySemaphores();
	DestroyCommandPool();
	DestroyLogicalDevice();
	DestroySurface();
	DestroyDebugMessenger();
	DestroyInstance();
}

void FramebufferResizeCallback(GLFWwindow* Window, int Width, int Height) {
	g_WinResized = true;
}

int main() {
	auto Draw = []() {
		unsigned int ImageIndex = -1;

		vkWaitForFences(g_Device, g_MaxFramesInFlight, g_InFlightFences.data(), VK_TRUE, UINT64_MAX);
		vkAcquireNextImageKHR(g_Device, g_Swapchain, UINT64_MAX, g_ImagesAvailable[g_CurrentFrame], VK_NULL_HANDLE, &ImageIndex);

		if (g_InFlightImages[g_CurrentFrame] != VK_NULL_HANDLE) {
			vkWaitForFences(g_Device, 1, &g_InFlightImages[g_CurrentFrame], VK_TRUE, UINT64_MAX);
		}

		g_InFlightImages[g_CurrentFrame] = g_InFlightFences[g_CurrentFrame];

		vkResetFences(g_Device, 1, &g_InFlightFences[g_CurrentFrame]);

		VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore SignalSemaphores[] = { g_RendersFinished[g_CurrentFrame] };
		VkSemaphore WaitSemaphores[] = { g_ImagesAvailable[g_CurrentFrame] };
		VkSwapchainKHR Swapchains[] = { g_Swapchain };
		
		static double StartTime = glfwGetTime();
		double CurrentTime = glfwGetTime();

		double Time = CurrentTime - StartTime;

		UniformBufferObject UBO{};
		UBO.Model = glm::rotate(UBO.Model, (float)Time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		UBO.View = glm::lookAt(glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		UBO.Projection = glm::perspective(glm::radians(45.0f), g_SwapchainExtent.width / (float)g_SwapchainExtent.height, 0.1f, 10.0f);
		UBO.Projection[1][1] *= -1.0f; // opengl y coord to vk

		void* Data;
		vkMapMemory(g_Device, g_UniformBufferMemories[ImageIndex], 0, sizeof(UBO), 0, &Data);
		memcpy(Data, &UBO, sizeof(UBO));
		vkUnmapMemory(g_Device, g_UniformBufferMemories[ImageIndex]);

		VkSubmitInfo SubmitInfo{};
		SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		SubmitInfo.pCommandBuffers = &g_CommandBuffers[ImageIndex];
		SubmitInfo.pSignalSemaphores = SignalSemaphores;
		SubmitInfo.pWaitSemaphores = WaitSemaphores;
		SubmitInfo.pWaitDstStageMask = WaitStages;
		SubmitInfo.signalSemaphoreCount = 1;
		SubmitInfo.commandBufferCount = 1;
		SubmitInfo.waitSemaphoreCount = 1;

		if (vkQueueSubmit(g_GraphicsQueue, 1, &SubmitInfo, g_InFlightFences[g_CurrentFrame]) != VK_SUCCESS) {
			std::cerr << "[ERROR] Failed to submit draw command buffer." << std::endl;
			return;
		}

		VkPresentInfoKHR PresentInfo{};
		PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		PresentInfo.pWaitSemaphores = SignalSemaphores;
		PresentInfo.pImageIndices = &ImageIndex;
		PresentInfo.pSwapchains = Swapchains;
		PresentInfo.waitSemaphoreCount = 1;
		PresentInfo.swapchainCount = 1;
		PresentInfo.pResults = nullptr;

		VkResult QueuePresentResult = vkQueuePresentKHR(g_PresentQueue, &PresentInfo);

		if (g_WinResized || QueuePresentResult == VK_SUBOPTIMAL_KHR || QueuePresentResult == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapchain();
			g_WinResized = false;
		}

		g_CurrentFrame = (g_CurrentFrame + 1) % g_MaxFramesInFlight;
	};

	auto CalculateTime = []() {
		CurrentTime = glfwGetTime();
		FPS++;

		if (CurrentTime - PreviousTime >= 1.0f) {
			std::string Title = "Vulkan renderer / ";
			Title += std::to_string(FPS);
			Title += " FPS";

			glfwSetWindowTitle(g_Window, Title.c_str());

			PreviousTime = CurrentTime;
			FPS = 0;
		}
	};

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	if (!glfwVulkanSupported()) {
		std::cerr << "[ERROR] This machine does not support Vulkan." << std::endl;
		glfwTerminate();
		return -1;
	}

	g_Window = glfwCreateWindow(g_WinWidth, g_WinHeight, "Vulkan Renderer / ? FPS", nullptr, nullptr);

	InitializeVulkan();

	PreviousTime = glfwGetTime();

	while (!glfwWindowShouldClose(g_Window)) {
		glfwPollEvents();
		Draw();
		CalculateTime();
	}

	vkDeviceWaitIdle(g_Device);

	Destroy();

	glfwDestroyWindow(g_Window);
	glfwTerminate();

	return 1;
}