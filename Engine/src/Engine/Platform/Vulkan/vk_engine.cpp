#include "pch.h"

#include "vk_engine.h"

// disable enum class scope
#pragma warning(push)
#pragma warning( disable : 26812 )

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <sdl2/SDL.h>
#include <sdl2/SDL_vulkan.h>

#include <glm/gtx/transform.hpp>

#include "Engine/Platform/Vulkan/vk_initializers.h"
#include "Engine/Platform/Vulkan/vk_textures.h"

#include <VkBootstrap.h>

#include <iostream>
#include <fstream>

#define SECOND_IN_NANOSECONDS 1000000000

constexpr bool bUseValidationLayers = true;

namespace
{
using namespace std;
#define VK_CHECK(x)															  \
		do																	  \
		{																	  \
		VkResult err = x;													  \
		if(err)																  \
		{																	  \
			std::cout << "Detected Vulkan error: " << err << std::endl;		  \
			abort();														  \
		}																	  \
		}																	  \
		while (0)															  \

} // anon namespace

void VulkanEngine::init()
{
	// load core vulkan structures
	init_vulkan();

	//create the swapchain
	init_swapchain();

	init_default_renderpass();

	init_framebuffers();

	init_commands();
	
	init_sync_structures();

	init_descriptors();

	init_pipelines();

	load_images();

	load_meshes();

	// load imgui - called from outside
	//init_imgui();

	init_scene();

	//everything went fine
	_isInitialized = true;
}

void VulkanEngine::load_images()
{
	Texture lostEmpire;

	vkutil::load_image_from_file(*this, "../Engine/assets/model/lost_empire-RGBA.png", lostEmpire.image);

	VkImageViewCreateInfo imageInfo = vkinit::imageview_create_info(VK_FORMAT_R8G8B8A8_SRGB, lostEmpire.image._image,
																	VK_IMAGE_ASPECT_COLOR_BIT);

	vkCreateImageView(_device, &imageInfo, nullptr, &lostEmpire.imageView);
	_loadedTextures["empire_diffuse"] = lostEmpire;
	_mainDeletionQueue.push_function([=]() 
	{
		vkDestroyImageView(_device, lostEmpire.imageView, nullptr);
	});

}

void VulkanEngine::init_vulkan()
{
	vkb::InstanceBuilder builder;
	
	//make the vulkan instance with basic debug features
	auto inst_ret = builder.set_app_name("Example Vulkan Application")
		.request_validation_layers(bUseValidationLayers) // TODO: Enable for debug only
		.use_default_debug_messenger()
		.require_api_version(1, 1, 0)
		.build();

	vkb::Instance vkb_inst = inst_ret.value();

	//store the instance 
	_instance = vkb_inst.instance;
	//store the debug messenger;
	_debug_messenger = vkb_inst.debug_messenger;

	// Get the surface of the window we opened with SDL
	SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

	// use vkboostrap to select a GPU.
	// We want a GPU that can write to the SDL surfae and supports vulkan 1.1
	vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 1)
		.set_surface(_surface)
		.select()
		.value();

	// create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };
	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the Vk Device handle used in the rest of the vulkan application
	_device = vkbDevice.device;
	_chosenGPU = physicalDevice.physical_device;

	//get properties
	vkGetPhysicalDeviceProperties(_chosenGPU, &_gpuProperties);
	std::cout << "The GPU has a minimum buffer alignment of " << _gpuProperties.limits.minUniformBufferOffsetAlignment << std::endl;

	// use vkBootstrap to get a graphics queue
	_graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	_graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

	// initlialize the memory allocator (VMA)
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = _chosenGPU;
	allocatorInfo.device = _device;
	allocatorInfo.instance = _instance;
	vmaCreateAllocator(&allocatorInfo, &_allocator);

	_mainDeletionQueue.push_function([=]() 
	{ 
		vmaDestroyAllocator(_allocator); 
	}
	);
}

void VulkanEngine::init_imgui()
{
	//1: create descriptor pool for IMGUI
	// the size of the pool is very oversize, but it's copied from imgui demo itself.
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.pNext = nullptr;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool));

	// 2: initialize imgui library

	////this initializes the core structures of imgui
	//ImGui::CreateContext();
	//ImGuiIO &io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	////this initializes imgui for SDL
	////ImGui_ImplSDL2_InitForVulkan(_window);

	//	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();

	//this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = _instance;
	init_info.PhysicalDevice = _chosenGPU;
	init_info.Device = _device;
	init_info.QueueFamily = _graphicsQueueFamily;
	init_info.Queue = _graphicsQueue;
	//init_info.PipelineCache = 
	init_info.DescriptorPool = _descriptorPool;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;


	//std::cout << "Render pass [" << _renderPass << "]\n";
	ImGui_ImplVulkan_Init(&init_info, _renderPass);

	//execute a gpu command to upload imgui font textures
	immediate_submit([&](VkCommandBuffer cmd)
	{
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
	});

	//clear font textures from cpu data
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	//add the destroy the imgui created structures
	_mainDeletionQueue.push_function([=]()
	{
		vkDestroyDescriptorPool(_device, imguiPool, nullptr);
		//ImGui_ImplVulkan_Shutdown();
	});
}

AllocatedBuffer VulkanEngine::create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;

	bufferInfo.size = allocSize;
	bufferInfo.usage = usage;

	VmaAllocationCreateInfo vmaAllocInfo{};
	vmaAllocInfo.usage = memoryUsage;


	AllocatedBuffer newBuffer{};

	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaAllocInfo,
							 &newBuffer._buffer, &newBuffer._allocation,
							 nullptr));

	return newBuffer;
}

void VulkanEngine::init_scene()
{
	
	RenderObject monkey;
	monkey.mesh = get_mesh("monkey");
	monkey.material = get_material("defaultmesh");
	monkey.transformMatrix = glm::mat4{ 1.0f };

	_renderables.push_back(monkey);

	RenderObject map;
	map.mesh = get_mesh("empire");
	map.material = get_material("texturedmesh");
	map.transformMatrix = glm::translate(glm::vec3{ 5.f, -10.f, 0.f });

	_renderables.push_back(map);


	for (int x = -20; x <= 20; x++)
	{
		for (int y = -20; y <= 20; y++)
		{
			RenderObject tri;
			tri.mesh = get_mesh("triangle");
			tri.material = get_material("defaultmesh");
			glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, glm::vec3{x, 0, y});
			glm::mat4 scale = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.2,0.2,0.2 });
			tri.transformMatrix = translation * scale;

			_renderables.push_back(tri);
		}
	}

	Material *texturedMat = get_material("texturedmesh");

//allcote the descriptor set for single-texture to use on the material
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &_singleTextureLayout;

	vkAllocateDescriptorSets(_device, &allocInfo, &texturedMat->textureSet);

	VkSamplerCreateInfo samplerInfo = vkinit::sampler_create_info(VK_FILTER_NEAREST);

	VkSampler blockySampler;
	vkCreateSampler(_device, &samplerInfo, nullptr, &blockySampler);
	_mainDeletionQueue.push_function([=]()
	{
		vkDestroySampler(_device, blockySampler, nullptr);
	});

	//write to the desciptor set so that it points to our empire_diffuse texture
	VkDescriptorImageInfo imageBufferInfo;
	imageBufferInfo.sampler = blockySampler;
	imageBufferInfo.imageView = _loadedTextures["empire_diffuse"].imageView;
	imageBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet  texture1 = vkinit::write_descriptor_image(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
																	texturedMat->textureSet,
																	&imageBufferInfo,
																	0);

	vkUpdateDescriptorSets(_device, 1, &texture1, 0, nullptr);
}


void VulkanEngine::init_swapchain()
{
	vkb::SwapchainBuilder swapchainBuilder{ _chosenGPU, _device, _surface };

	// Select Surface Format
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	VkSurfaceFormatKHR format{};
	format.colorSpace = requestSurfaceColorSpace;	// sRGB colour
	format.format = VK_FORMAT_R8G8B8A8_UNORM;		// RGBA values

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		.set_desired_format(format) // get this format that matches ImGUI, generally should be okay
		//.use_default_format_selection()
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR) // TODO: Change to mailbox present mode
		.set_desired_extent(_windowExtent.width, _windowExtent.height)
		.build()
		.value();

	//store swapchain and its related images
	_swapchain = vkbSwapchain.swapchain;
	_swapchainImages = vkbSwapchain.get_images().value();
	_swapchainImageViews = vkbSwapchain.get_image_views().value();

	_swapchainImageFormat = vkbSwapchain.image_format;

	//depth image size matches window
	VkExtent3D depthImageExtent = {
		_windowExtent.width,
		_windowExtent.height,
		1
	};

	// hardcoding the depth format to 32 bit float
	_depthFormat = VK_FORMAT_D32_SFLOAT;

	// the depth image will be an image with the format we selected and depth attachemnt usage flags
	VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

	//for the depth image, we want to allocate  it from GPU local memory
	VmaAllocationCreateInfo dimg_allocinfo{};
	dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//allocate and create the image
	vmaCreateImage(_allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

	//build an image view for the depth image to use for rendering
	VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

	VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView));

	_mainDeletionQueue.push_function([=]() {
		vkDestroySwapchainKHR(_device, _swapchain, nullptr);
		vkDestroyImageView(_device,_depthImageView,nullptr);
		vmaDestroyImage(_allocator, _depthImage._image, _depthImage._allocation);
	});
}

void VulkanEngine::init_commands()
{
	// Create a command pool for commands submitted to the graphics queue
	// we also want the pool to allow for resetting of individual command buffers;
	VkCommandPoolCreateInfo commandPoolInfo= vkinit::command_pool_create_info(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	
	for (size_t i = 0; i < FRAME_OVERLAP; i++)
	{
		VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_frames[i]._commandPool));

		// Allocate the default command buffer that we will use for rendering
		VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);

		VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_frames[i]._mainCommandBuffer));

		_mainDeletionQueue.push_function([=]()
		{
			vkDestroyCommandPool(_device, _frames[i]._commandPool, nullptr);
		});
	}	

	//create a pool for upload context
	VkCommandPoolCreateInfo uploadCommandPoolInfo = vkinit::command_pool_create_info(_graphicsQueueFamily);
	VK_CHECK(vkCreateCommandPool(_device, &uploadCommandPoolInfo, nullptr, &_uploadContext._commandPool));
	_mainDeletionQueue.push_function([=]()
	{
		vkDestroyCommandPool(_device, _uploadContext._commandPool, nullptr);
	});
}

void VulkanEngine::init_default_renderpass()
{
	// the render pass will use this color attachment.
	VkAttachmentDescription color_attachment{};
	// the attachment will have the format needed by the swapchain
	color_attachment.format = _swapchainImageFormat;
	// 1 sample, no MSAA
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	// we clear when this attachment is loaded
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// we keep the attachment stored when the renderpass ends
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//we dont care about stencil
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	// we dont know or care about the starting layout of the attachment
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	//after the renderpass ends the image has to be on a layout ready for display
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref{};
	//attachment number will be idnex into the pAttachments array in the parent renderpass itself
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


	// depth attachment
	VkAttachmentDescription depth_attachment{};
	depth_attachment.flags = 0;
	depth_attachment.format = _depthFormat;
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_ref{};
	depth_attachment_ref.attachment = 1;
	depth_attachment_ref.layout= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;	

	// we are going to create 1 subpass, which is the minimum
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	// hook the depth attachment into the subpass
	subpass.pDepthStencilAttachment = &depth_attachment_ref;

	// 1 dependency which is from "outside" into the subpass. and we can read or write color
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// array of 2 attachments, one for color, one for depth
	VkAttachmentDescription attachments[2] = {
	color_attachment, 
	depth_attachment
	};

	VkRenderPassCreateInfo render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	// connect the color attahcment o the info
	render_pass_info.attachmentCount = 2;
	render_pass_info.pAttachments = &attachments[0];
	// connect the subpass to the info;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	VK_CHECK(vkCreateRenderPass(_device, &render_pass_info, nullptr, &_renderPass));

	_mainDeletionQueue.push_function([=]()
	{
		vkDestroyRenderPass(_device, _renderPass,nullptr); 
	});
}

void VulkanEngine::init_framebuffers()
{
	// create the frambuffers for the swapchain images. this will connect the render-pass to the images for rendering
	VkFramebufferCreateInfo fb_info = vkinit::framebuffer_create_info(_renderPass,_windowExtent);

	// Grab how many images we have in the swap chain
	const uint32_t swapchain_imageCount = static_cast<const uint32_t>(_swapchainImages.size());
	_framebuffers = std::vector<VkFramebuffer>(swapchain_imageCount);

	// create framebuffers for each of the swapchain image views
	for (size_t i = 0; i < swapchain_imageCount; i++)
	{
		VkImageView attachments[2];
		attachments[0] = _swapchainImageViews[i];
		attachments[1] = _depthImageView;

		fb_info.pAttachments = &attachments[0];
		fb_info.attachmentCount = 2;
		VK_CHECK(vkCreateFramebuffer(_device, &fb_info, nullptr, &_framebuffers[i]));

		_mainDeletionQueue.push_function([=]() 
		{ 
			vkDestroyFramebuffer(_device,_framebuffers[i],nullptr); 
			vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
		});
	}
}

void VulkanEngine::init_sync_structures()
{
	// Create synchronization structures
	// [One] fences to control when gpu has finished rendering the frame
	// [Two] semaphores to sync rendering with swapchain

	// we want to craete the fence with the create signaled flag so we can wait on it before using it on a gpu command
	VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);

	//for semaphores we dont need any flags
	VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

	for (size_t i = 0; i < FRAME_OVERLAP; i++)
	{
		VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_frames[i]._renderFence));

		// Enqueue destruction of fence
		_mainDeletionQueue.push_function([=]()
		{
			vkDestroyFence(_device, _frames[i]._renderFence, nullptr);
		});

		VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._presentSemaphore));
		VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._renderSemaphore));

		// Enqueue the destruction of semaphores
		_mainDeletionQueue.push_function([=]()
		{
			vkDestroySemaphore(_device, _frames[i]._presentSemaphore, nullptr);
			vkDestroySemaphore(_device, _frames[i]._renderSemaphore, nullptr);
		}
		);
	}

	VkFenceCreateInfo uploadFenceCreateInfo = vkinit::fence_create_info();
	VK_CHECK(vkCreateFence(_device, &uploadFenceCreateInfo, nullptr, &_uploadContext._uploadFence));
	_mainDeletionQueue.push_function([=]()
	{
		vkDestroyFence(_device, _uploadContext._uploadFence, nullptr);
	});

}

bool VulkanEngine::load_shader_module(const char *filePath, VkShaderModule *outShaderModule)
{
	//open the file. With cursor at the end
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		return false;
	}

	//find what the size of the file is by looking up the location of the cursor
	//because the cursor is at the end, it gives the size directly in bytes
	size_t fileSize = (size_t)file.tellg();

	// spir-v expects the buffer to be on uint32_t, so make sure to reserve an int vector big enough for the entire file
	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

	// put cursor at file beginning
	file.seekg(0);

	//load entire file into the buffer
	file.read((char *)buffer.data(), fileSize);
	
	// now that the file is loaded in buffer we can close it
	file.close();

	// create a new shader module using the buffer we loaded
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	// code size has to be in bytes. so multiply the ints in the buffer by size of int to know the real size of the buffer
	createInfo.codeSize = buffer.size() * sizeof(uint32_t);
	createInfo.pCode = buffer.data();

	// check that creation goes well
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		return false;
	}
	*outShaderModule = shaderModule;
	return true;

}

void VulkanEngine::init_descriptors()
{
	std::vector<VkDescriptorPoolSize> sizes = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10},
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10},
		{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10},
		//add combined image sampler descriptor types to the pool
		{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , 10}
	};

	VkDescriptorPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = static_cast<uint32_t>(sizes.size());
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_device, &pool_info, nullptr, &_descriptorPool);
	_mainDeletionQueue.push_function([=]()
	{
		vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	});

	// it's a uniform buffer binding
	//we use it from the vertex shader
	//binding at 0
	VkDescriptorSetLayoutBinding cameraBind= vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
																				  VK_SHADER_STAGE_VERTEX_BIT,
																				  0);
	// binding at 1
	VkDescriptorSetLayoutBinding sceneBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
																				  VK_SHADER_STAGE_VERTEX_BIT| VK_SHADER_STAGE_FRAGMENT_BIT,
																				  1);
	VkDescriptorSetLayoutBinding bindings[] = { cameraBind,sceneBind };

	VkDescriptorSetLayoutCreateInfo setInfo{};
	setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setInfo.pNext = nullptr;
	// we are going to have 1 binding
	setInfo.bindingCount = 2;
	// no flags
	setInfo.flags = 0;
	// point to camer buffer binding
	setInfo.pBindings = bindings;

	vkCreateDescriptorSetLayout(_device, &setInfo, nullptr, &_globalSetLayout);


	VkDescriptorSetLayoutBinding objectBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
																				   VK_SHADER_STAGE_VERTEX_BIT, 0);

	VkDescriptorSetLayoutCreateInfo set2Info{};
	set2Info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	set2Info.pNext = nullptr;
	set2Info.bindingCount = 1;
	set2Info.flags = 0;
	set2Info.pBindings = &objectBind;
	
	vkCreateDescriptorSetLayout(_device, &set2Info, nullptr, &_objectSetLayout);

	VkDescriptorSetLayoutBinding textureBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
																					VK_SHADER_STAGE_FRAGMENT_BIT, 0);

	VkDescriptorSetLayoutCreateInfo set3Info{};
	set3Info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	set3Info.pNext = nullptr;
	set3Info.bindingCount = 1;
	set3Info.flags = 0;
	set3Info.pBindings = &textureBind;

	vkCreateDescriptorSetLayout(_device, &set3Info, nullptr, &_singleTextureLayout);

	const size_t sceneParamBufferSize = FRAME_OVERLAP * pad_uniform_buffer_size(sizeof(GPUSceneData));

	_sceneParameterBuffer = create_buffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

	_mainDeletionQueue.push_function([=]()
	{
		vmaDestroyBuffer(_allocator, _sceneParameterBuffer._buffer, _sceneParameterBuffer._allocation);
		vkDestroyDescriptorSetLayout(_device, _globalSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(_device, _objectSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(_device, _singleTextureLayout, nullptr);
	});


	for (size_t i = 0; i < FRAME_OVERLAP; i++)
	{
		const int MAX_OBJECTS=10000;
		_frames[i].cameraBuffer = create_buffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
		_frames[i].objectBuffer = create_buffer(sizeof(GPUObjectData) * MAX_OBJECTS, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		// allocate one descriptor set for each frame
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		// using the pool we just set
		allocInfo.descriptorPool = _descriptorPool;
		// only 1 descriptor
		allocInfo.descriptorSetCount = 1;
		// using the global data layout
		allocInfo.pSetLayouts = &_globalSetLayout;
		
		vkAllocateDescriptorSets(_device, &allocInfo, &_frames[i].globalDescriptor);

		VkDescriptorSetAllocateInfo objectSetAlloc{};
		objectSetAlloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		objectSetAlloc.pNext = nullptr;
		objectSetAlloc.descriptorPool = _descriptorPool;
		objectSetAlloc.descriptorSetCount = 1;
		objectSetAlloc.pSetLayouts = &_objectSetLayout;

		vkAllocateDescriptorSets(_device, &objectSetAlloc, &_frames[i].objectDescriptor);

		//information about the buffer we want to point to in the descriptor
		VkDescriptorBufferInfo cameraInfo{};
		// it will be at the camera buffer
		cameraInfo.buffer = _frames[i].cameraBuffer._buffer;
		// at 0 offset
		cameraInfo.offset = 0;
		// of the size of a camera datat struct
		cameraInfo.range = sizeof(GPUCameraData);

		VkDescriptorBufferInfo sceneInfo{};
		sceneInfo.buffer = _sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkDescriptorBufferInfo objectBufferInfo{};
		objectBufferInfo.buffer = _frames[i].objectBuffer._buffer;
		objectBufferInfo.offset = 0;
		objectBufferInfo.range = sizeof(GPUObjectData) * MAX_OBJECTS;

		// we are going to write into binding number 0
		// of the global descriptor
		// and the type  is uniform buffer
		VkWriteDescriptorSet cameraWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _frames[i].globalDescriptor, &cameraInfo, 0);
		VkWriteDescriptorSet sceneWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, _frames[i].globalDescriptor, &sceneInfo, 1);
		VkWriteDescriptorSet objectWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _frames[i].objectDescriptor, &objectBufferInfo, 0);

		VkWriteDescriptorSet setWrites[]{ cameraWrite, sceneWrite, objectWrite};
		
		vkUpdateDescriptorSets(_device, 3, setWrites, 0, nullptr);



		_mainDeletionQueue.push_function([=]()
		{
			vmaDestroyBuffer(_allocator, _frames[i].objectBuffer._buffer, _frames[i].objectBuffer._allocation);
			vmaDestroyBuffer(_allocator, _frames[i].cameraBuffer._buffer, _frames[i].cameraBuffer._allocation);
		});
	}	

	
}

void VulkanEngine::init_pipelines()
{
	// compile colored triangle modules
	VkShaderModule colorMeshShader;
	if (!load_shader_module("../Engine/assets/shaders/default_lit.frag.spv", &colorMeshShader))
	{
		std::cout << "Error while building the triangle fragment shader module" << std::endl;
	}
	else
	{
		std::cout << "Triangle fragment shader module successfully loaded" << std::endl;
	}

	VkShaderModule texturedMeshShader;
	if (!load_shader_module("../Engine/assets/shaders/textured_lit.frag.spv", &texturedMeshShader))
	{
		std::cout << "Error while building the textured fragment shader module" << std::endl;
	}
	else
	{
		std::cout << "Triangle fragment shader module successfully loaded" << std::endl;
	}

	VkShaderModule meshVertShader;
	if (!load_shader_module("../Engine/assets/shaders/tri_mesh.vert.spv", &meshVertShader))
	{
		std::cout << "Error while building the mesh triangle vertex shader module" << std::endl;
	}
	else
	{
		std::cout << "Mesh Triangle vertex shader module successfully loaded" << std::endl;
	}

	// build the stage-create-info for both vertex and fragment stages. this lets the pipeline know the shadermodules per stage
	PipelineBuilder pipelineBuilder;
	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));
	
	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, colorMeshShader));

	// build the pipeline layout that controls the inputs/outputs of the shader
	// we are not using the desciptor sets or other systems yet, so no need to use anything other than empty default
	VkPipelineLayoutCreateInfo mesh_pipeline_layout_info = vkinit::pipeline_layout_create_info();

	//setup push constant
	VkPushConstantRange push_constant;

	// this push constant range starts at the beginning
	push_constant.offset = 0;
	//this push constant range takes up the size of a MeshPushConstants struct
	push_constant.size = sizeof(MeshPushConstants);
	//this push constant is accesible only in the vertex shader
	push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	mesh_pipeline_layout_info.pPushConstantRanges = &push_constant;
	mesh_pipeline_layout_info.pushConstantRangeCount = 1;

	VkDescriptorSetLayout setLayouts[] = { _globalSetLayout, _objectSetLayout };

	//hook the global set layout
	mesh_pipeline_layout_info.setLayoutCount = 2;
	mesh_pipeline_layout_info.pSetLayouts = setLayouts;

	VkPipelineLayout meshPipeLayout;
	VK_CHECK(vkCreatePipelineLayout(_device, &mesh_pipeline_layout_info, nullptr, &meshPipeLayout));

	//create pipline for textured mesh, which has 3 descriptor sets
	// we start from the normal mesh layout
	VkPipelineLayoutCreateInfo textured_pipeline_layout_info = mesh_pipeline_layout_info;

	VkDescriptorSetLayout texturedSetLayouts[] = { _globalSetLayout, _objectSetLayout, _singleTextureLayout };


	textured_pipeline_layout_info.setLayoutCount = 3;
	textured_pipeline_layout_info.pSetLayouts = texturedSetLayouts;

	VkPipelineLayout texturedPipeLayout;
	VK_CHECK(vkCreatePipelineLayout(_device, &textured_pipeline_layout_info, nullptr, &texturedPipeLayout));



	//hook the push constants layout 
	pipelineBuilder._pipelineLayout = meshPipeLayout;
	
	//vertex input controls how to read vertices from vertex buffer. we arent using it yet
	pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

	//input assembly is the configuration for drawing triangle lists,  strips, or individual points.
	// we are just going to draw a triangle list
	pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	// build viewport and scissor from the swapchain extents
	pipelineBuilder._viewport.x = 0.0f;
	pipelineBuilder._viewport.y = 0.0f; // perhaps can start this from +height and lead to -height
	pipelineBuilder._viewport.width = static_cast<float>(_windowExtent.width);
	pipelineBuilder._viewport.height = static_cast<float>(_windowExtent.height); // inverting this might be possible and invert viewport Y
	pipelineBuilder._viewport.minDepth = 0.0f;
	pipelineBuilder._viewport.maxDepth = 1.0f;

	pipelineBuilder._scissor.offset = { 0 , 0 };
	pipelineBuilder._scissor.extent = _windowExtent;

	//configure the rasterizer to draw filled triangles
	pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

	//we dont use multisampling so just run the default one
	pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

	// a single blend attachment with no blending and writing to RGBA
	pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

	// default depth testing
	pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

	//build the mesh pipeline
	VertexInputDescription vertexDescription = Vertex::get_vertex_description();

	// connect the pipeline builder vertex input info to the one we get from Vertex
	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexDescription.attributes.size());

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexDescription.bindings.size());

	VkPipeline meshPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

	create_material(meshPipeline, meshPipeLayout, "defaultmesh");


	// Create pipeline for textured drawing
	pipelineBuilder._shaderStages.clear();
	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, texturedMeshShader));

	// connect the new pipeline layout to the pipeline builder
	pipelineBuilder._pipelineLayout = texturedPipeLayout;
	VkPipeline texPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);
	create_material(texPipeline, texturedPipeLayout, "texturedmesh");


	// destroy all shader modules as we are done with them
	vkDestroyShaderModule(_device, meshVertShader, nullptr);
	vkDestroyShaderModule(_device, colorMeshShader, nullptr);
	vkDestroyShaderModule(_device, texturedMeshShader, nullptr);

	// Enqueue destruction of pipelines
	_mainDeletionQueue.push_function([=]()
	{ 
		//destroy the pipelines we have created
		vkDestroyPipeline(_device, meshPipeline, nullptr);
		vkDestroyPipeline(_device, texPipeline, nullptr);
		//destroy the layout that they are using
		vkDestroyPipelineLayout(_device, meshPipeLayout, nullptr);
		vkDestroyPipelineLayout(_device, texturedPipeLayout, nullptr);
	});
}

void VulkanEngine::immediate_submit(std::function<void(VkCommandBuffer cmd)> &&function)
{
	// allocate the default command buffer that we will use for the instant commmands
	VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_uploadContext._commandPool, 1);

	VkCommandBuffer cmd;
	VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &cmd));

	//begin the command buffer recording. we will use this command buffer exactly once. so we want to let vulkan know
	VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

	//execute the function
	function(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd));

	VkSubmitInfo submit = vkinit::submit_info(&cmd);

	// submit command buffer to the queue and execute it
	// _uploadFence will now block until graphics commands finish execution
	VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, _uploadContext._uploadFence));

	vkWaitForFences(_device, 1, &_uploadContext._uploadFence, true, 999999999);
	vkResetFences(_device, 1, &_uploadContext._uploadFence);

	// clear the command pool. this will free the commmand buffer too
	vkResetCommandPool(_device, _uploadContext._commandPool, 0);


}

FrameData &VulkanEngine::get_current_frame()
{
	return _frames[_frameNumber % FRAME_OVERLAP];
}

void VulkanEngine::load_meshes()
{
	Mesh triMesh{};
	//make array 3 vertices long
	triMesh._vertices.resize(3);

	//vertex positions
	triMesh._vertices[0].position = {  1.0f, 1.0f,0.0f };
	triMesh._vertices[1].position = { -1.0f, 1.0f,0.0f };
	triMesh._vertices[2].position = {  0.0f,-1.0f,0.0f };

	//vertex colours, all green
	triMesh._vertices[0].color = { 0.0f,1.0f,0.0f }; // pure green
	triMesh._vertices[1].color = { 0.0f,1.0f,0.0f }; // pure green
	triMesh._vertices[2].color = { 0.0f,1.0f,0.0f }; // pure green

	//load monkey
	Mesh monkeyMesh{};
	monkeyMesh.load_from_obj("../Engine/assets/model/monkey_smooth.obj");
							
	Mesh lostEmpire{};		
	lostEmpire.load_from_obj("../Engine/assets/model/lost_empire.obj");

	// make sure all meshes are sent to the gpu
	upload_mesh(triMesh);
	upload_mesh(monkeyMesh);
	upload_mesh(lostEmpire);

	//note that we are copying them.
	// eventually we  will delete the hardcoded _monkey and _triangle meshes.
	_meshes["monkey"] = monkeyMesh;
	_meshes["triangle"] = triMesh;
	_meshes["empire"] = lostEmpire;

}

void VulkanEngine::upload_mesh(Mesh &mesh)
{
	
	const size_t bufferSize = mesh._vertices.size() * sizeof(Vertex);
	// allocate staging buffer
	VkBufferCreateInfo stagingBufferInfo{};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	//this is the total size, in bytes,  of the buffer we are allocating
	stagingBufferInfo.size = bufferSize;
	// this buffer is going to be used as a Vertex buffer
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	// let the VMA library know that this data should be writable to CPU but also readable by GPU
	VmaAllocationCreateInfo vmaAllocInfo{};
	vmaAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	AllocatedBuffer stagingBuffer;

	// allocate the buffer
	VK_CHECK(vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaAllocInfo,
							 &stagingBuffer._buffer,
							 &stagingBuffer._allocation,
							 nullptr));

	//copy vertex data 
	void *data;
	vmaMapMemory(_allocator, stagingBuffer._allocation, &data);
	memcpy(data, mesh._vertices.data(), mesh._vertices.size() * sizeof(Vertex));
	vmaUnmapMemory(_allocator, stagingBuffer._allocation);

	//allocate vertex buffer
	VkBufferCreateInfo vertexBufferInfo{};

	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.pNext = nullptr;
	// this is the total size in bytes of the buffer we are allocating
	vertexBufferInfo.size = bufferSize;
	// this buffer is going to be used as vertex buffer
	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	//let the vma library know that this data should be gpu native
	vmaAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	VK_CHECK(vmaCreateBuffer(_allocator, &vertexBufferInfo, &vmaAllocInfo,
					&mesh._vertexBuffer._buffer,
					&mesh._vertexBuffer._allocation,
					nullptr));

	//both buffers created send command
	immediate_submit([=](VkCommandBuffer cmd)
	{
		VkBufferCopy copy{};
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = bufferSize;
		vkCmdCopyBuffer(cmd, stagingBuffer._buffer, mesh._vertexBuffer._buffer, 1, &copy);
	});

	// Add the destruction of triangle mesh buffer to the deletion queue
	_mainDeletionQueue.push_function([=]() 
	{
		vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
	}
	);
	vmaDestroyBuffer(_allocator, stagingBuffer._buffer, stagingBuffer._allocation);

}

Material *VulkanEngine::create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string &name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	_materials[name] = mat;
	return &_materials[name];
}

Material *VulkanEngine::get_material(const std::string &name)
{
	// search for the object, and return nullptr if not found
	auto it = _materials.find(name);
	if (it == _materials.end()) 
	{
	return nullptr;
	}
	else
	{
		return &(*it).second;
	}
}

Mesh *VulkanEngine::get_mesh(const std::string &name)
{
		// search for the object, and return nullptr if not found
	auto it = _meshes.find(name);
	if (it == _meshes.end())
	{
		return nullptr;
	}
	else
	{
		return &(*it).second;
	}
}

void VulkanEngine::draw_objects(VkCommandBuffer cmd, RenderObject *first, int count)
{
	// make a model view matrix for rendering the object
	//camera view
	glm::vec3 camPos = { 0.0f,-6.0f,-10.0f };

	glm::mat4 view = glm::translate(glm::mat4{ 1.0f }, camPos);
	//camera projection
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(_windowExtent.width / _windowExtent.height), 0.1f, 200.0f);
	projection[1][1] *= -1.0f; // can consider flipping viewport instead of this

	//fill a GPU camera struct
	GPUCameraData camData;
	camData.projection = projection;
	camData.view = view;
	camData.viewproj = projection * view;

	//copy it to buffer
	void *data;
	vmaMapMemory(_allocator, get_current_frame().cameraBuffer._allocation, &data);
	memcpy(data, &camData, sizeof(GPUCameraData));
	vmaUnmapMemory(_allocator, get_current_frame().cameraBuffer._allocation);

	float framed = (_frameNumber / 120.0f);
	_sceneParameters.ambientColor = { sin(framed),0,cos(framed),1 };

	char *sceneData;
	vmaMapMemory(_allocator, _sceneParameterBuffer._allocation, (void **)&sceneData);
	int frameIndex = _frameNumber % FRAME_OVERLAP;

	sceneData += pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex;
	memcpy(sceneData, &_sceneParameters, sizeof(GPUSceneData));
	vmaUnmapMemory(_allocator, _sceneParameterBuffer._allocation);

	void *objectData;
	vmaMapMemory(_allocator, get_current_frame().objectBuffer._allocation, &objectData);

	GPUObjectData *objectSSBO = static_cast<GPUObjectData *>(objectData);

	for (size_t i = 0; i < count; i++)
	{
		RenderObject &object = first[i];
		objectSSBO[i].modelMatrix = object.transformMatrix;
	}

	vmaUnmapMemory(_allocator, get_current_frame().objectBuffer._allocation);

	Mesh *lastMesh = nullptr;
	Material *lastMaterial = nullptr;

	for (size_t i = 0; i < count; i++)
	{
		RenderObject &object = first[i];

		//only bind the pipeline if it doesnt match with the already bound one
		if (object.material != lastMaterial)
		{
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipeline);
			lastMaterial = object.material;

			//offset for our scene buffer
			uint32_t uniform_offset = static_cast<uint32_t>(pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex);

			//bind the descriptor set when changing pipeline
			vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout,
									0, 1, &get_current_frame().globalDescriptor, 1, &uniform_offset);

			//object data descriptor
			vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout,
									1, 1, &get_current_frame().objectDescriptor, 0, nullptr);

			if (object.material->textureSet != VK_NULL_HANDLE)
			{
				//texture descriptor
				vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout,
										2, 1, &object.material->textureSet, 0, nullptr);
			}
		}

		MeshPushConstants constants;
		constants.render_matrix = object.transformMatrix;

		//upload the mesh to GPU via push constants
		vkCmdPushConstants(cmd, object.material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

		//only bind the mesh if its a different one from last bind
		if (object.mesh != lastMesh)
		{
			//bind mesh to vertex buffer with offset 0
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->_vertexBuffer._buffer, &offset);
			lastMesh = object.mesh;
		}
		// we can now draw
		vkCmdDraw(cmd, static_cast<uint32_t>(object.mesh->_vertices.size()), 1, 0, static_cast<uint32_t>(i));
	}

}


void VulkanEngine::cleanup()
{	
	if (_isInitialized) {
		
		// make sure the GPU has stopped doings stuff
		vkDeviceWaitIdle(_device);

		// go through and destroy all the creations
		_mainDeletionQueue.flush();


		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		vkDestroyDevice(_device, nullptr);
		vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);
		vkDestroyInstance(_instance, nullptr);
		//SDL_DestroyWindow(_window);
	}
}

void VulkanEngine::RenderFrame()
{
	// wait until GPU has finished rendering last frame, timeout of 1 second
	VK_CHECK(vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, SECOND_IN_NANOSECONDS));
	VK_CHECK(vkResetFences(_device, 1, &get_current_frame()._renderFence));

	// now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
	VK_CHECK(vkResetCommandBuffer(get_current_frame()._mainCommandBuffer, 0));

	// Request image from swapchain, 1 second timeout;
	uint32_t swapchainImageIndex;
	VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, SECOND_IN_NANOSECONDS, get_current_frame()._presentSemaphore, nullptr, &swapchainImageIndex));
	
	VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

	// begin the command buffer recording. We will use this command to buffer once
	VkCommandBufferBeginInfo cmdBegInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBegInfo));

	// make clear colour from frame number. this will flash with a 120*pi frame period;
	VkClearValue clearValue{};
	float flash = abs(sin(_frameNumber / 120.0f));
	clearValue.color = { 0.0f,0.0f,flash,1.0f };

	//clear depth at 1
	VkClearValue depthClear{};
	depthClear.depthStencil.depth = 1.0f;

	// start the main render pass
	// we will use the clear color from above, and frame buffer of the index the swapchain gave us
	VkRenderPassBeginInfo rpInfo = vkinit::renderpass_begin_info(_renderPass,_windowExtent,_framebuffers[swapchainImageIndex]);	
	
	rpInfo.clearValueCount = 2;
	VkClearValue clearValues[]{ clearValue, depthClear };

	rpInfo.pClearValues = &clearValues[0];

	vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

	// [-- rendering commands --]
	draw_objects(cmd, _renderables.data(), static_cast<int>(_renderables.size()));
	// [-- end rendering commands --]

	// imgui rendering commands
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

	vkCmdEndRenderPass(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd));

	//prepare the submission to the queue
	// we want to wait on the _presentSemaphore , as that semaphore is signaled when the swapchain is ready
	// we will still signal the _renderSempahore to signal that rendering has finished

	VkSubmitInfo submit = vkinit::submit_info(&cmd);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submit.pWaitDstStageMask = &waitStage;

	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &get_current_frame()._presentSemaphore;

	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &get_current_frame()._renderSemaphore;

	//submit the cmd buffer to the queue and execute it
	// _renderFence will now block until graphics command finishes execution
	VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, get_current_frame()._renderFence));

	// this will put the image we just rendered into the visible window
	// we want to wait on the _renderSemaphore for that.
	// as it's necessary that drawing commands have finished before the imageis displayed to the user
	VkPresentInfoKHR presentInfo = vkinit::present_info();

	presentInfo.pSwapchains = &_swapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &get_current_frame()._renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &swapchainImageIndex;
	VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

	// increase number of frames drawn
	++_frameNumber;


}

void VulkanEngine::run()
{
	SDL_Event e;
	bool bQuit = false;

	//main loop
	while (!bQuit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);

			//close the window when user alt-f4s or clicks the X button			
			if (e.type == SDL_QUIT)
			{
				bQuit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					_selectShader += 1;
					if (_selectShader > 1)
					{
						_selectShader = 0;
					}
				}
			}
		}


		//imgui new frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window);

		ImGui::NewFrame();

		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Begin("text");
		//ImGui::Image((ImTextureID)(intptr_t)imgID, {1024.0f,1024.0f});
		ImGui::Text("Hello world");
		ImGui::End();

		// render imgui windows
		ImGui::Render();
		  // Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		RenderFrame();	

	}
}

VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass)
{
	// make the viewport state from our stored viewport and scissor
	// at the moment we wont support multiple viewports or scissors
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr;

	viewportState.viewportCount = 1;
	viewportState.pViewports = &_viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &_scissor;

	// setup dummy color blending. we arent using transparent objects yet
	// the blending is just "no blend", but we do write to the color attachment
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.pNext = nullptr;

	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &_colorBlendAttachment;

	// build the actual pipeline
	// we now use all of the info structs we have been writing into this one to create the pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = nullptr;

	pipelineInfo.stageCount = static_cast<uint32_t>(_shaderStages.size());
	pipelineInfo.pStages = _shaderStages.data();
	pipelineInfo.pVertexInputState = &_vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &_inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &_rasterizer;
	pipelineInfo.pMultisampleState = &_multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &_depthStencil;

	VkPipeline newPipeline;
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
	{
		std::cout << "Failed to create a pipeline\n";
		return VK_NULL_HANDLE;
	}
	else
	{
		return newPipeline;
	}
}

// disable enum class scope
#pragma warning(push)
