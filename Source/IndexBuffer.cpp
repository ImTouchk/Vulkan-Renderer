#include "..\Include\Definitions.h"

// External variable declarations
VkDeviceMemory g_IndexBufferMemory;
VkBuffer g_IndexBuffer;

// External functions
void CreateIndexBuffer() {
	VkDeviceSize BufferSize = sizeof(Indices[0]) * Indices.size();

	VkDeviceMemory StagingMemory;
	VkBuffer StagingBuffer;

	CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingMemory);

	CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, g_IndexBuffer, g_IndexBufferMemory);

	void* Data;
	vkMapMemory(g_Device, StagingMemory, 0, BufferSize, 0, &Data);
	memcpy(Data, Indices.data(), BufferSize);
	vkUnmapMemory(g_Device, StagingMemory);

	CopyBuffer(StagingBuffer, g_IndexBuffer, BufferSize);

	vkDestroyBuffer(g_Device, StagingBuffer, nullptr);
	vkFreeMemory(g_Device, StagingMemory, nullptr);
}

void DestroyIndexBuffer() {
	vkDestroyBuffer(g_Device, g_IndexBuffer, nullptr);
	vkFreeMemory(g_Device, g_IndexBufferMemory, nullptr);
}