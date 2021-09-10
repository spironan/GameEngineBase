#pragma once

#include "Engine/ECS/Component.h"
#include "Engine/Renderer/Camera.h"

#include <rttr/type>

namespace engine
{

class SceneCamera final : public Component, public Camera 
{

public:
	/*-----------------------------------------------------------------------------*/
	/* Constructors and Destructors                                                */
	/*-----------------------------------------------------------------------------*/
	SceneCamera() = delete;
	SceneCamera(SceneCamera const&) = default;
	SceneCamera(SceneCamera&&) = default;
	SceneCamera& operator=(SceneCamera const&) = default;
	SceneCamera& operator=(SceneCamera&&) = default;
	virtual ~SceneCamera() override = default;

	SceneCamera(Entity _entityID, bool _active = true);

	/*-----------------------------------------------------------------------------*/
	/* Getter Functions                                                            */
	/*-----------------------------------------------------------------------------*/
	ProjectionType GetProjectionType() const { return m_ProjectionType; }
	
	float GetPerspectiveVerticalFOV() const { return oom::degrees(m_PerspectiveFOV); }
	float GetPerspectiveNearClip() const	{ return m_PerspectiveNear; }
	float GetPerspectiveFarClip() const		{ return m_PerspectiveFar; }
	float GetOrthographicSize() const		{ return m_OrthographicSize; }
	float GetOrthographicNearClip() const	{ return m_OrthographicNear; }
	float GetOrthographicFarClip() const	{ return m_OrthographicFar; }

	/*-----------------------------------------------------------------------------*/
	/* Setter Functions                                                            */
	/*-----------------------------------------------------------------------------*/
	void SetPerspective(float verticalFOV, float nearClip = 0.01f, float farClip = 1000.0f);
	void SetOrthographic(float size, float nearClip = -1.0f, float farClip = 1.0f);

	void SetProjectionType(ProjectionType type)			{ m_ProjectionType = type; }
	
	void SetPerspectiveVerticalFOV(float verticalFov)	{ m_PerspectiveFOV = oom::radians(verticalFov); }
	void SetPerspectiveNearClip(float nearClip)			{ m_PerspectiveNear = nearClip; }
	void SetPerspectiveFarClip(float farClip)			{ m_PerspectiveFar = farClip; }
	void SetOrthographicSize(float size)				{ m_OrthographicSize = size; }
	void SetOrthographicNearClip(float nearClip)		{ m_OrthographicNear = nearClip; }
	void SetOrthographicFarClip(float farClip)			{ m_OrthographicFar = farClip; }

	/*-----------------------------------------------------------------------------*/
	/* Member Functions                                                            */
	/*-----------------------------------------------------------------------------*/
	
	/*********************************************************************************//*!
	\brief    Updates the camera's projection matrix using the set ProjectionType.
				Uses the width and height of the viewport as reference to create the aspect ratio

	\param    width width of the viewport
	\param    height height of the viewport
	*//**********************************************************************************/
	void UpdateViewportSize(uint32_t width, uint32_t height);

	RTTR_ENABLE();

private:
	ProjectionType m_ProjectionType = ProjectionType::Orthographic;

	float m_PerspectiveFOV = oom::radians(45.0f);
	float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 1000.0f;

	float m_OrthographicSize = 500.0f;
	float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
};

}

