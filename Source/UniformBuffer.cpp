#include "..\Include\Definitions.h"

// External variable declarations
std::vector<VkDeviceMemory> g_UniformBufferMemories;
std::vector<VkDescriptorSet> g_DescriptorSets;
std::vector<VkBuffer> g_UniformBuffers;

VkDescriptorSetLayout g_DescriptorLayout;
VkDescriptorPool g_DescriptorPool;

// External functions
void CreateDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding DescriptorLayoutBinding{};
	DescriptorLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	DescriptorLayoutBinding.pImmutableSamplers = nullptr;
	DescriptorLayoutBinding.descriptorCount = 1;
	DescriptorLayoutBinding.binding = 0;

	VkDescriptorSetLayoutCreateInfo DescriptorCreateInfo{};
	DescriptorCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorCreateInfo.pBindings = &DescriptorLayoutBinding;
	DescriptorCreateInfo.bindingCount = 1;

	if (vkCreateDescriptorSetLayout(g_Device, &DescriptorCreateInfo, nullptr, &g_DescriptorLayout) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create descriptor set layout." << std::endl;
		return;
	}
}

void DestroyDescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(g_Device, g_DescriptorLayout, nullptr);
}

void CreateDescriptorPool() {
	VkDescriptorPoolSize PoolSize{};
	PoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	PoolSize.descriptorCount = g_SwapchainImages.size();

	VkDescriptorPoolCreateInfo PoolCreateInfo{};
	PoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	PoolCreateInfo.maxSets = g_SwapchainImages.size();
	PoolCreateInfo.pPoolSizes = &PoolSize;
	PoolCreateInfo.poolSizeCount = 1;

	if (vkCreateDescriptorPool(g_Device, &PoolCreateInfo, nullptr, &g_DescriptorPool) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to create descriptor pool." << std::endl;
		return;
	}
}

void DestroyDescriptorPool() {
	vkDestroyDescriptorPool(g_Device, g_DescriptorPool, nullptr);
}

void CreateDescriptorSets() {
	std::vector<VkDescriptorSetLayout> Layouts(g_SwapchainImages.size(), g_DescriptorLayout);

	VkDescriptorSetAllocateInfo DescriptorAllocateInfo{};
	DescriptorAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	DescriptorAllocateInfo.descriptorSetCount = g_SwapchainImages.size();
	DescriptorAllocateInfo.descriptorPool = g_DescriptorPool;
	DescriptorAllocateInfo.pSetLayouts = Layouts.data();

	g_DescriptorSets.resize(g_SwapchainImages.size());

	if (vkAllocateDescriptorSets(g_Device, &DescriptorAllocateInfo, g_DescriptorSets.data()) != VK_SUCCESS) {
		std::cerr << "[ERROR] Failed to allocate descriptor set." << std::endl;
		return;
	}

	for (unsigned int i = 0; i < g_SwapchainImages.size(); i++) {
		VkDescriptorBufferInfo DescriptorBufferInfo{};
		DescriptorBufferInfo.range = sizeof(UniformBufferObject);
		DescriptorBufferInfo.buffer = g_UniformBuffers[i];
		DescriptorBufferInfo.offset = 0;

		VkWriteDescriptorSet WriteDescriptorSet{};
		WriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		WriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorSet.pBufferInfo = &DescriptorBufferInfo;
		WriteDescriptorSet.dstSet = g_DescriptorSets[i];
		WriteDescriptorSet.pTexelBufferView = nullptr;
		WriteDescriptorSet.pImageInfo = nullptr;
		WriteDescriptorSet.descriptorCount = 1;
		WriteDescriptorSet.dstArrayElement = 0;
		WriteDescriptorSet.dstBinding = 0;

		vkUpdateDescriptorSets(g_Device, 1, &WriteDescriptorSet, 0, nullptr);
	}
}

void CreateUniformBuffers() {
	VkDeviceSize BufferSize = sizeof(UniformBufferObject);

	g_UniformBuffers.resize(g_SwapchainImages.size());
	g_UniformBufferMemories.resize(g_SwapchainImages.size());

	for (unsigned int i = 0; i < g_SwapchainImages.size(); i++) {
		CreateBuffer(BufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, g_UniformBuffers[i], g_UniformBufferMemories[i]);
	}
}

void DestroyUniformBuffers() {
	for (unsigned int i = 0; i < g_SwapchainImages.size(); i++) {
		vkDestroyBuffer(g_Device, g_UniformBuffers[i], nullptr);
		vkFreeMemory(g_Device, g_UniformBufferMemories[i], nullptr);
	}
}