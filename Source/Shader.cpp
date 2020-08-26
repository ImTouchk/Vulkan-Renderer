#include "..\Include\Definitions.h"

// External functions
VkShaderModule CreateShaderModule(const std::vector<char>& Code) {
	VkShaderModuleCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	CreateInfo.pCode = reinterpret_cast<const uint32_t*>(Code.data());
	CreateInfo.codeSize = Code.size();

	VkShaderModule Shader;
	if (vkCreateShaderModule(g_Device, &CreateInfo, nullptr, &Shader) != VK_SUCCESS) {
		std::cerr << "[ERROR] Couldn't create shader module." << std::endl;
	}
	return Shader;
}

std::vector<char> ReadFile(const std::string& Name) {
	std::ifstream File(Name, std::ios::ate | std::ios::binary);

	if (!File.is_open()) {
		std::cerr << "[ERROR] Couldn't open file." << std::endl;
		return std::vector<char>();
	}

	unsigned int FileSize = File.tellg();
	std::vector<char> Buffer(FileSize);
	File.seekg(0);
	File.read(Buffer.data(), FileSize);
	File.close();
	return Buffer;
}