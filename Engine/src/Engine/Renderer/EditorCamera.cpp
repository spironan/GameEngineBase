#include "pch.h"

#include "Engine/Core/Input.h"
#include "EditorCamera.h"

#include "Engine/Core/Log.h"

namespace engine
{

	std::unique_ptr<engine::EditorCamera> EditorCamera::g_editorCam{ std::make_unique<engine::EditorCamera>() };

EditorCamera::EditorCamera(const oom::mat4& projectionMatrix)
	:Camera(projectionMatrix),
	m_lastMousePosition{ Input::GetMousePosition().first, Input::GetMousePosition().second }
{
	
}

void EditorCamera::OnUpdate(float dt)
{
	const auto imouse = Input::GetMousePosition();
	const oom::vec2 mouse{ imouse.first,imouse.second };
	const oom::vec2 delta = (mouse - m_lastMousePosition) * 0.002f;

	if ( 1)
	{
		if (Input::IsMouseButtonDown(MouseCode::ButtonMiddle))
		{
			MousePan(delta);
		}
	}

	m_lastMousePosition = mouse;

	UpdateView();
}

const oom::vec3 EditorCamera::GetUp() const
{
	//TODO : proper calculations for 3D support
	return oom::vec3(0, 1, 0);
}

const oom::vec3 EditorCamera::GetRight() const
{
	//TODO : proper calculations for 3D support
	return oom::vec3(1, 0, 0);
}

const oom::vec3 EditorCamera::GetForward() const
{
	//TODO : proper calculations for 3D support
	return oom::vec3(0, 0, -1);
}

void EditorCamera::UpdateMouse(int32_t xRel, int32_t yRel)
{

	/*if (m_cameraMode == CameraMode::ORTHO)
	{
		if (yRel > 0)
		{
			MoveUp();
		}
		else if (yRel < 0)
		{
			MoveDown();
		}

		if (xRel > 0)
		{
			MoveLeft();
		}
		else if (xRel < 0)
		{
			MoveRight();
		}

	}*/

}

void EditorCamera::UpdateView()
{
	
	oom::vec3 w = GetForward();
	oom::vec3 u = GetRight();
	oom::vec3 v = GetUp();

	m_view = oom::mat4{
		u.x,					v.x,					w.x,					0.0f,
		u.y,					v.y,					w.y,					0.0f,
		u.z,					v.z,					w.z,					0.0f,
		-oom::dot(u,m_position),-oom::dot(v,m_position),-oom::dot(w,m_position),1.0f
	};
}



void EditorCamera::MousePan(const oom::vec2& delta)
{
	m_position += -GetRight() * delta.x* m_panSpeed;
	m_position += GetUp() * delta.y * m_panSpeed;
	//LOG_ENGINE_INFO("{0},{1},{2}", m_position.x, m_position.y, m_position.z);
}

}// End namespace engine