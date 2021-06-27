#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#include <../vendor/glm/glm/glm.hpp>
#include <sdl2/SDL_events.h>

#include "Engine/Platform/Vulkan/vk_types.h"
#include "Engine/Debug/cvars.h"

struct DebugCamera
{
	VkExtent2D _windowExtent;

	AutoCVar_Vec3 CVAR_CamPos{ "camera.position", "Position X, Y, Z", { 2.f,4.f,8.f }, CVarFlags::EditFloatDrag };
	AutoCVar_Vec3 CVAR_Depth{ "camera.orthoClip", "Near Clip, Far Clip, <unused>", { -1.f,10000.f,999.0f }, CVarFlags::EditFloatDrag };
	AutoCVar_Vec3 CVAR_fov{ "camera.PerspectiveFOV", "FOV in degrees, Near Clip, Far Clip",{70.0f, 0.1f, 5000.0f}, CVarFlags::EditFloatDrag };
	AutoCVar_Float CVAR_orthoHeight{ "camera.orthoZoom", "orthographic Height",15.0f, CVarFlags::EditFloatDrag };
	AutoCVar_Int CVAR_ortho{ "camera.ToggleOrtho", "ortho toggling", 0, CVarFlags::EditCheckbox };

	glm::vec3 velocity{};
	glm::vec3 inputAxis{};
	float m_cameraSpeed{ 1000.0f };

	//void moveCamera();

	float pitch{ 0 }; //up-down rotation
	float yaw{ 0 }; //left-right rotation

	bool b_slowDown = { false };
	bool b_Locked = { true };
	bool b_Ortho = {false};
	bool b_perFrame = { true };

	void process_input_event(SDL_Event* ev);
	void update_camera(float deltaSeconds);
	void update_cameraExtent(VkExtent2D val);

	void set_per_frame_input(bool status);

	//Enables the camera movement
	void enable_movement(bool status);
	void slow_camera(bool status);
	void set_orthographic(bool status);
	void set_perspective(bool status);

	void move_fowards(bool invert = false);
	void move_backwards(bool invert = false);
	void move_right(bool invert = false);
	void move_left(bool invert = false);
	void move_up(bool invert = false);
	void move_down(bool invert = false);

	void update_mouse_relative(int32_t xRel, int32_t yRel);

	glm::mat4 get_view_matrix();
	glm::mat4 get_projection_matrix();
	glm::mat4 get_rotation_matrix();
};

