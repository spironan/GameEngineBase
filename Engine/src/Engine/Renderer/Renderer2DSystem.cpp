/************************************************************************************//*!
\file          Renderer2DSystem.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains the implementation of the rendering 2D
				system which renders quads to screen in batch using ECS systems


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "pch.h"
#include "Renderer2DSystem.h"

#include "Renderer2D.h"

#include "Engine/ECS/ECS.h"
#include "Engine/ECS/WorldManager.h"

#include "Engine/Renderer/Sprite2D.h"

#include "Engine/Transform/Transform3D.h"
#include "Engine/Transform/TransformSystem.h"
#include "engine/PhysicsCollision/PhysicsCollision.h"

namespace engine
{
	void Renderer2DSystem::SetCamera(const Camera& cam, const glm::vec3& position)
	{
		m_cam = &cam;

		m_projection = m_cam->GetProjection();
		//auto transform = WorldManager::GetActiveWorld().GetComponent<Transform3D>(cam.GetEntity());

		//glm::vec3 pos = m_transform->GetGlobalPosition();
		m_view = {
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			-position.x, -position.y,	-position.z,	1.0f
		};

		//m_view = glm::inverse(glm::translate(glm::mat4{ 1.0f },m_transform->GetGlobalPosition()) * m_transform->GetGlobalRotationMatrix());
		//m_view = glm::inverse(m_transform->GetGlobalMatrix());
		m_viewProj = m_projection * m_view;
	}

	void Renderer2DSystem::Update()
	{
		auto view = m_ECS_Manager.GetComponentView<Transform3D, Sprite2D>();
		engine::Renderer2D::BeginScene( m_viewProj, m_view);
		for (auto [transform, sprite] : view)
		{
			Renderer2D::DrawRotatedQuad(transform.GetGlobalPosition(),
										transform.GetGlobalScale(),
										transform.GetGlobalRotationDeg(),
										sprite.GetTexture(), 1.0f,
										sprite.GetColor());
			Renderer2D::DrawCircle(transform.GetGlobalPosition(),
								   transform.GetGlobalRotationDeg(),
								   transform.GetGlobalScale().x / 2.f,
								   sprite.GetColor());
		}
		engine::Renderer2D::EndScene();
	}

};
