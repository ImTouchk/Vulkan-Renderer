#include "..\Include\Definitions.h"

// External variable declarations
VkDeviceMemory g_VertexBufferMemory;
VkBuffer g_VertexBuffer;

// Internal function declarations
unsigned int FindMemoryType(unsigned int TypeFilter, VkMemoryPropertyFlags Properties);

// External functions
void CreateVertexBuffer() {
	VkDeviceSize BufferSize = sizeof(Vertices[0]) * Vertices.size();
	VkDeviceMemory StagingMemory;
	VkBuffer StagingBuffer;

	CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingMemory);

	CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, g_VertexBuffer, g_VertexBufferMemory);

	void* Data;
	vkMapMemory(g_Device, StagingMemory, 0, BufferSize, 0, &Data);
	memcpy(Data, Vertices.data(), BufferSize);
	vkUnmapMemory(g_Device, StagingMemory);

	CopyBuffer(StagingBuffer, g_VertexBuffer, BufferSize);

	vkDestroyBuffer(g_Device, StagingBuffer, nullptr);
	vkFreeMemory(g_Device, StagingMemory, nullptr);
}

void DestroyVertexBuffer() {
	vkDestroyBuffer(g_Device, g_VertexBuffer, nullptr);
	vkFreeMemory(g_Device, g_VertexBufferMemory, nullptr);
}

// Internal functions
unsigned int FindMemoryType(unsigned int TypeFilter, VkMemoryPropertyFlags Properties) {
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(g_PhysicalDevice, &MemoryProperties);

	for (unsigned int i = 0; i < MemoryProperties.memoryTypeCount; i++) {
		if (TypeFilter & (1 << i) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties) {
			return i;
		}
	}

	std::cerr << "[ERROR] Failed to find suitable memory type." << std::endl;
	return 0;
}

void CreateBuffer(VkDeviceSize Size, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags PropertyFlags, VkBuffer& Buffer, VkDeviceMemory& Memory) {
	VkBufferCreateInfo BufferCreateInfo{};
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	BufferCreateInfo.usage = UsageFlags;
	BufferCreateInfo.size = Size;

	if (vkCreateBuffer(g_Device, &BufferCreateInfo, nullptr, &Buffer) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create buffer." << std::endl;
		return;
	}

	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(g_Device, Buffer, &MemoryRequirements);

	VkMemoryAllocateInfo MemoryAllocateInfo{};
	MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
	MemoryAllocateInfo.memoryTypeIndex = FindMemoryType(MemoryRequirements.memoryTypeBits, PropertyFlags);

	if (vkAllocateMemory(g_Device, &MemoryAllocateInfo, nullptr, &Memory) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to allocate buffer memory." << std::endl;
	}

	vkBindBufferMemory(g_Device, Buffer, Memory, 0);
}

void CopyBuffer(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size) {
	VkCommandBufferAllocateInfo BufferAllocateInfo{};
	BufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	BufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	BufferAllocateInfo.commandPool = g_CommandPool;
	BufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer CommandBuffer;
	vkAllocateCommandBuffers(g_Device, &BufferAllocateInfo, &CommandBuffer);

	VkCommandBufferBeginInfo BufferBeginInfo{};
	BufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(CommandBuffer, &BufferBeginInfo);

	VkBufferCopy CopyRegion{};
	CopyRegion.srcOffset = 0;
	CopyRegion.dstOffset = 0;
	CopyRegion.size = Size;

	vkCmdCopyBuffer(CommandBuffer, Source, Destination, 1, &CopyRegion);
	vkEndCommandBuffer(CommandBuffer);

	VkSubmitInfo SubmitInfo{};
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pCommandBuffers = &CommandBuffer;
	SubmitInfo.commandBufferCount = 1;

	vkQueueSubmit(g_GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(g_GraphicsQueue);

	vkFreeCommandBuffers(g_Device, g_CommandPool, 1, &CommandBuffer);
}