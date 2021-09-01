
#include "pch.h"
#include "SceneCamera.h"

#include <rttr/registration.h>

namespace engine
{

	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<SceneCamera>("Camera")
		.property("Camera Type", &SceneCamera::GetProjectionType, &SceneCamera::SetProjectionType)
		
		// ortho camera
		.property("Size", &SceneCamera::GetOrthographicSize, &SceneCamera::SetOrthographicSize)
		.property("Near clipping plane", &SceneCamera::GetOrthographicNearClip, &SceneCamera::SetOrthographicNearClip)
		.property("Far clippinng plane", &SceneCamera::GetOrthographicNearClip, &SceneCamera::SetOrthographicNearClip)
		
		// perspective camera
		.property("Vertical Fov", &SceneCamera::GetPerspectiveVerticalFOV, &SceneCamera::SetPerspectiveVerticalFOV)
		.property("Near clipping plane", &SceneCamera::GetPerspectiveNearClip, &SceneCamera::SetPerspectiveNearClip)
		.property("Far clipping plane", &SceneCamera::GetPerspectiveFarClip, &SceneCamera::SetPerspectiveFarClip);
	}


SceneCamera::SceneCamera(Entity _entityID, bool _active)
	:Component{ _entityID, _active },
	Camera{}
{
	UpdateViewportSize(1600, 900);
}

void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Perspective;
	m_PerspectiveFOV = verticalFOV;
	m_PerspectiveNear = nearClip;
	m_PerspectiveFar = farClip;
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Orthographic;
	m_OrthographicSize = size;
	m_OrthographicNear = nearClip;
	m_OrthographicFar = farClip;
}

void SceneCamera::UpdateViewportSize(uint32_t width, uint32_t height)
{
	float ar = static_cast<float>(width) / height;

	switch (m_ProjectionType)
	{
		case ProjectionType::Perspective:
			m_projection = /*oom::perspectiveFovLH_ZO*/oom::perspective(ar, m_PerspectiveFOV, m_PerspectiveNear, m_PerspectiveFar);
			break;
		case ProjectionType::Orthographic:
			float aspectRatio = (float)width / (float)height;
			float w = m_OrthographicSize * aspectRatio;
			float h = m_OrthographicSize;
			//m_projection = oom::ortho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f);
			m_projection = oom::ortho(aspectRatio, w * 0.5f, -1, 1);
			break;
	}
}

}// end namespace engine