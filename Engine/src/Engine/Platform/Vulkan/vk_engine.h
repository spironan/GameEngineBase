// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

// disable enum class scope
#pragma warning(push)
#pragma warning( disable : 26812 )

#include "Engine/Platform/Vulkan/vk_types.h"
#include "Engine/Platform/Vulkan/vk_mesh.h"

#include <vector>
#include <deque>
#include <functional>
#include <unordered_map>

#include <../vendor/glm/glm/mat4x4.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"

#include <sdl2/SDL.h>

#include "Engine/Renderer/DebugCamera.h"

constexpr unsigned int FRAME_OVERLAP = 2;

struct UploadContext
{
	VkFence _uploadFence;
	VkCommandPool _commandPool;

	UploadContext() = default;
};

struct GPUCameraData
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewproj;

	GPUCameraData() = default;
};

struct FrameData
{
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	AllocatedBuffer cameraBuffer;
	VkDescriptorSet globalDescriptor;

	AllocatedBuffer objectBuffer;
	VkDescriptorSet objectDescriptor;

	FrameData() = default;
};

struct Texture
{
	AllocatedImage image;
	VkImageView imageView;

	Texture() = default;
};

struct Material
{
	//we store the pipeline and layout by value, not pointer
	//they are 64 bit handles to internal driver structures so storing pointers isnt useful
	VkDescriptorSet textureSet{ VK_NULL_HANDLE }; // texture defaulted to null
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;

	Material() = default;
};

struct RenderObject
{
	Mesh *mesh;
	Material *material;
	glm::mat4 transformMatrix;

	RenderObject() = default;
};

struct MeshPushConstants
{
	glm::vec4 data;
	glm::mat4 render_matrix;

	MeshPushConstants() = default;
};

struct DeletionQueue
{
	std::deque < std::function<void()>> deletors;

	void push_function(std::function<void()> &&function)
	{
		deletors.push_back(function);
	}
	void flush()
	{
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend() ; ++it)
		{
			(*it)();// call functors
		}
		deletors.clear();
	}
};

class PipelineBuilder
{
public:
	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineLayout _pipelineLayout;
	VkPipelineDepthStencilStateCreateInfo _depthStencil;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);

	PipelineBuilder() = default;
};

struct GPUSceneData
{
	glm::vec4 fogColor; //w is for exponent
	glm::vec4 fogDistances; // x is for min, y is for max, zw unused;
	glm::vec4 ambientColor;
	glm::vec4 sunlightDireciton; // w for sun power
	glm::vec4 sunlightColor;

	GPUSceneData() = default;
};

struct GPUObjectData
{
	glm::mat4 modelMatrix;

	GPUObjectData() = default;
};




class VulkanEngine {
public:
	VulkanEngine() = default;
	AutoCVar_Int CVAR_wireframe{ "Engine.ToggleWireframe", "wireframe toggling", 0, CVarFlags::EditCheckbox };
	DebugCamera _camera;

	void SetWindow(SDL_Window *window) {
		_window={ window };
		int w, h;
		SDL_GetWindowSize(_window,&w,&h);
		_windowExtent.width = w; _windowExtent.height = h;
	}

	void SetWindowResized()
	{
		_windowResized = true;
	}

	UploadContext _uploadContext;
	//texture hash map
	std::unordered_map<std::string, Texture> _loadedTextures;

	void immediate_submit(std::function<void(VkCommandBuffer cmd)> &&function);

	bool _isInitialized{ false };
	int _frameNumber {0};
	uint32_t _currentImageIndex{0};

	VmaAllocator _allocator; // VMA library allocator
	DeletionQueue _mainDeletionQueue;

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkPhysicalDeviceProperties _gpuProperties;
	VkDevice _device;
	VkSurfaceKHR _surface;

	VkSwapchainKHR _swapchain;	
	// Image format expected by the window system
	VkFormat _swapchainImageFormat;
	// Array of images from the swapchain
	std::vector<VkImage> _swapchainImages;
	// Array of image views from the swapchain
	std::vector<VkImageView> _swapchainImageViews;

	//format for depth image
	VkFormat _depthFormat;
	VkImageView _depthImageView;
	AllocatedImage _depthImage;

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkRenderPass _renderPass;

	std::vector<VkFramebuffer> _framebuffers;
	
	// frame storage
	FrameData _frames[FRAME_OVERLAP];

	//getter for the frame we are rendering to right now.
	FrameData& get_current_frame();

	VkDescriptorSetLayout _globalSetLayout;
	VkDescriptorSetLayout _objectSetLayout;
	VkDescriptorSetLayout _singleTextureLayout;
	VkDescriptorPool _descriptorPool;

	GPUSceneData _sceneParameters;
	AllocatedBuffer _sceneParameterBuffer;
	
	//default array of renderable objects
	std::vector<RenderObject> _renderables;

	std::unordered_map<std::string, Material> _materials;
	std::unordered_map<std::string, Mesh> _meshes;

	VkExtent2D _windowExtent{ 1024 , 720 };

	struct SDL_Window* _window{ nullptr };
	bool _windowResized{ false };
	bool _recreateSwapchain{ false };

	int _selectShader{ 0 };

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void RenderFrame();

	void PresentFrame();

	//run main loop
	void run();

	// reacreate swapchain
	void RecreateSwapchain();

	//create materials and add it to the map
	Material *create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string &name);

	AllocatedBuffer VulkanEngine::create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	//returns nullptr if cant be found
	Material *get_material(const std::string &name);

	//returns nullptr if can't be found
	Mesh *get_mesh(const std::string &name);

	//draw function
	void draw_objects(VkCommandBuffer cmd, RenderObject *first, int count);

	void load_images();

	void init_imgui();

	void RenderCVAR();

	DebugCamera* getCam() { return &_camera; }

private: 

	void init_vulkan();

	void init_swapchain(VkSwapchainKHR oldSwapchain = nullptr);

	void init_commands();

	void init_default_renderpass();

	void init_framebuffers();

	void init_sync_structures();

	void init_descriptors();

	void init_pipelines();

	void load_meshes();

	void upload_mesh(Mesh& mesh);

	void init_scene();

	void get_window_extent()
	{
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		_windowExtent.width = w; _windowExtent.height = h;
	}


	//loads a shader module from a spir-v file. Returns false if it errors
	bool load_shader_module(const char *filePath, VkShaderModule *outShaderModule);

	size_t pad_uniform_buffer_size(size_t originalSize)
	{
		//calculate required alignment based on minimum device offset alignment
		size_t minUboAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
		size_t alignedSize = originalSize;
		if (minUboAlignment > 0)
		{
			// So this is some bitwise operations that calculates the minimum alignment size needed
			// the first part of the operation is taking the size of our item and giving it the minimum size.
			// then we minus one from that to kind of "round down" the number.
			// lastly we do a bitwise AND operation with the complement of the number. 
			// for example  [0000 1000] becomes [0000 0111] then negated becomes [1111 1000]. 
			// So this helps us compare which bits match up to the size containers
			alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
		}
		return alignedSize;
	}
};

//disable enum class scope
#pragma warning(pop)
