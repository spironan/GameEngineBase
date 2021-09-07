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

#include "Engine/PhysicsCollision/Colliders.h"
#include "Engine/PhysicsCollision/ColliderCore.h"

#include "Engine/Renderer/Debug/ColliderDebugDraw.h"

namespace engine
{
	void Renderer2DSystem::SetCamera(const Camera& cam, const oom::vec3& position)
	{
		m_cam = &cam;

		m_projection = m_cam->GetProjection();
		//auto transform = WorldManager::GetActiveWorld().GetComponent<Transform3D>(cam.GetEntity());

		//oom::vec3 pos = m_transform->GetGlobalPosition();
		m_view = {
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			-position.x, -position.y,	-position.z,	1.0f
		};

		//m_view = oom::inverse(oom::translate(oom::mat4{ 1.0f },m_transform->GetGlobalPosition()) * m_transform->GetGlobalRotationMatrix());
		//m_view = oom::inverse(m_transform->GetGlobalMatrix());
		m_viewProj = m_projection * m_view;
	}

	void Renderer2DSystem::Update()
	{
		
		engine::Renderer2D::BeginScene( m_viewProj, m_view);

		DrawSprites();
		
		DrawDebug();
		
		engine::Renderer2D::EndScene();
	}

	void Renderer2DSystem::DrawSprites()
	{
		auto view = m_ECS_Manager.GetComponentView<Transform3D, Sprite2D>();
		for (auto [transform, sprite] : view)
		{
			Renderer2D::DrawRotatedQuad(transform.GetGlobalPosition(),
										transform.GetGlobalScale(),
										transform.GetGlobalRotationDeg(),
										sprite.GetTexture(), 1.0f,
										sprite.GetColor());
		}
	}

	void Renderer2DSystem::DrawDebug()
	{
		auto view = m_ECS_Manager.GetComponentView<Transform3D, Collider2D, ColliderDebugDraw>();
		for (auto [transform, collider, debugCol] : view)
		{
			switch (collider.GetNarrowPhaseCollider())
			{
				case ColliderType::BOX:
					{
						auto box = collider.GetComponent<BoxCollider2D>().GetGlobalBounds();
						Renderer2D::DrawAABB2D(box,
											   debugCol.GetColor());
					}break;
				case ColliderType::CIRCLE:
					{
						Renderer2D::DrawCircle(transform.GetGlobalPosition(),
											   transform.GetGlobalRotationDeg(),
											   collider.GetComponent<CircleCollider2D>().GetGlobalBounds().radius,
											   debugCol.GetColor());
					}break;
				default:
					LOG_ENGINE_ERROR("No collider debug type implemented!");
					break;
			}			
		}
	}

};
