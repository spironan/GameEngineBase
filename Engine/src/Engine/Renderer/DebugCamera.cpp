#include "pch.h"

#include "DebugCamera.h"
#include "sdl2/SDL.h"
#include <iostream>
#include <cmath>
#include <glm/gtx/transform.hpp>

void DebugCamera::process_input_event(SDL_Event* ev)
{
	// Hides mouse cursor if user holds down right mouse button
	if (ev->type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev->button.button == SDL_BUTTON_RIGHT)
		{			
			enable_movement(false);
		}
	}
	if (ev->type == SDL_MOUSEBUTTONUP)
	{
		if (ev->button.button == SDL_BUTTON_RIGHT)
		{					
			enable_movement(true);
		}
	}
	
	if (ev->type == SDL_KEYDOWN)
	{
		switch (ev->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_w:
				move_fowards();
				break;
			case SDLK_DOWN:
			case SDLK_s:
				move_backwards();
				break;
			case SDLK_LEFT:
			case SDLK_a:
				move_left();
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				move_right();
				break;
			case SDLK_q:
				move_down();
				break;

			case SDLK_e:
				move_up();
				break;
			case SDLK_LSHIFT:
				slow_camera(true);
				break;
		}
	}
	else if (ev->type == SDL_KEYUP)
	{
		switch (ev->key.keysym.sym)
		{
			case SDLK_UP:
			case SDLK_w:
				move_fowards(true);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				move_backwards(true);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				move_left(true);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				move_right(true);
				break;
			case SDLK_q:
				move_down(true);
				break;

			case SDLK_e:
				move_up(true);
				break;
			case SDLK_LSHIFT:
				slow_camera(false);
				break;
		}
	}
	else if (ev->type == SDL_MOUSEMOTION)
	{
		update_mouse_relative(ev->motion.xrel, ev->motion.yrel);
	}	
}

void DebugCamera::update_camera(float deltaSeconds)
{
	// if camera locked reset input
	if (b_Locked)
	{
		inputAxis = {};
	}
	inputAxis = glm::clamp(inputAxis, { -1.0,-1.0,-1.0 }, { 1.0,1.0,1.0 });
	
	const float cam_vel = 0.001f + !b_slowDown * 0.01;
	glm::vec3 forward = { 0,0, -cam_vel };
	glm::vec3 right = { cam_vel,0,0 };

	glm::vec3 up;
	up = { 0,cam_vel,0 };

	glm::mat4 cam_rot = get_rotation_matrix();

	forward = cam_rot * glm::vec4(forward, 0.f);
	right = cam_rot * glm::vec4(right, 0.f);

	velocity = inputAxis.x * forward + inputAxis.y * right + inputAxis.z * up;

	velocity *= (b_Ortho?m_cameraSpeed*2.0f:m_cameraSpeed) * deltaSeconds;

	CVAR_CamPos.Set(CVAR_CamPos.Get()+ velocity);
	
	//reset input at end of frame
	if (b_perFrame)
	{
		inputAxis={};
	}

}

void DebugCamera::update_cameraExtent(VkExtent2D val)
{
	_windowExtent = val;
}

void DebugCamera::set_per_frame_input(bool status)
{
	b_perFrame = status;
}

void DebugCamera::enable_movement(bool status)
{
	// Returns the cursor to the middle of the screen. perhaps can cache this when activating
	// SDL_WarpMouseInWindow(m_pWindow, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	SDL_SetRelativeMouseMode(status?SDL_FALSE:SDL_TRUE);
	b_Locked = status;
}

void DebugCamera::slow_camera(bool status)
{
	b_slowDown = status;
}

void DebugCamera::set_orthographic(bool status)
{
	b_Ortho = status;
	if (b_Ortho)
	{
		CVAR_CamPos.Set({ 0.0f,0.0f,0.0f });
	}
}

void DebugCamera::set_perspective(bool status)
{
	b_Ortho = !status;
}

void DebugCamera::move_fowards(bool invert)
{
	if (b_Ortho)
	{
		move_up(invert);
		return;
	}

	if (invert)
	{
	inputAxis.x -= 1.f;
	}
	else
	{
	inputAxis.x += 1.f;
	}
}

void DebugCamera::move_backwards(bool invert)
{
	move_fowards(!invert);
}

glm::mat4 DebugCamera::get_view_matrix()
{
	glm::vec3 camPos = CVAR_CamPos.Get();

	glm::mat4 cam_rot = (get_rotation_matrix());

	glm::mat4 view = glm::translate(glm::mat4{ 1 }, camPos) * cam_rot;

	//we need to invert the camera matrix
	view = glm::inverse(view);

	return view;
}

glm::mat4 DebugCamera::get_projection_matrix()
{
	b_Ortho = CVAR_ortho.Get();
	if (b_Ortho)
	{
		float ar = static_cast<float>(_windowExtent.width)/ static_cast<float>(_windowExtent.height);	
		float height = CVAR_orthoHeight.GetFloat();
		glm::mat4 pro = glm::ortho(-ar*height, ar*height, -height, height,CVAR_Depth.Get().x, CVAR_Depth.Get().y);
			//pro[1][1] *= -1;
			return pro;
	}
	else
	{
		glm::mat4 pro = glm::perspective(glm::radians(CVAR_fov.Get().x), static_cast<float>(_windowExtent.width) / _windowExtent.height, CVAR_fov.Get().y, CVAR_fov.Get().z);
		//pro[1][1] *= -1;
		return pro;
	}
}

glm::mat4 DebugCamera::get_rotation_matrix()
{
	glm::mat4 pitch_rot;
	if (b_Ortho)
	{
		glm::mat4 yaw_rot = glm::rotate(glm::mat4{ 1 }, 0.f, { 0,-1,0 });
		pitch_rot = glm::rotate(glm::mat4{ yaw_rot }, 0.f, { -1,0,0 });
	}
	else
	{
		glm::mat4 yaw_rot = glm::rotate(glm::mat4{ 1 }, yaw, { 0,-1,0 });
		pitch_rot = glm::rotate(glm::mat4{ yaw_rot }, pitch, { -1,0,0 });
	}
	

	return pitch_rot;
}

void DebugCamera::move_left(bool invert)
{
	move_right(!invert);
}

void DebugCamera::move_up(bool invert)
{
	if (invert)
	{
		inputAxis.z -= 1.f;
	}
	else
	{
		inputAxis.z += 1.f;
	}
}

void DebugCamera::move_down(bool invert)
{
	move_up(!invert);
}

void DebugCamera::update_mouse_relative(int32_t xRel, int32_t yRel)
{
	if (!b_Locked)
	{
		pitch += yRel * 0.003f;
		yaw += xRel * 0.003f;
	}

	if (b_Ortho)
	{
		if (yRel < 0)
		{
			move_down();
		}
		else if (yRel > 0)
		{
			move_up();
		}

		if (xRel < 0)
		{
			move_right();
		}
		else if (xRel > 0)
		{
			move_left();
		}
	}
}

void DebugCamera::move_right(bool invert)
{
	if (invert)
	{
		inputAxis.y -= 1.f;
	}
	else
	{
		inputAxis.y += 1.f;
	}
}
