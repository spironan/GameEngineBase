#pragma once

#include "Platform/Vulkan/vk_types.h"
#include "Platform/Vulkan/vk_engine.h"

namespace vkutil
{
	bool load_image_from_file(VulkanEngine &engine, const char *file, AllocatedImage &outImage);
}
class vk_textures
{
};

