#pragma once

#include "Engine/Platform/Vulkan/vk_types.h"
//#include <../vendor/glm/glm/vec3.hpp>
//#include <../vendor/glm/glm/vec2.hpp>
#include <oom/oom.hpp>

#include <vector>

struct VertexInputDescription
{
	std::vector<VkVertexInputBindingDescription> bindings;
	std::vector<VkVertexInputAttributeDescription> attributes;

	VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex
{
	oom::vec3 position;
	oom::vec3 normal;
	oom::vec3 color;
	oom::vec2 uv;

	static VertexInputDescription get_vertex_description();
};

struct Mesh
{
	std::vector<Vertex> _vertices;
	AllocatedBuffer _vertexBuffer;

	bool load_from_obj(const char *filename);
};


