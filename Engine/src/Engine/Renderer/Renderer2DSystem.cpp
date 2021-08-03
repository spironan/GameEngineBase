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

#include "Engine/Renderer/Sprite2D.h"

#include "Engine/Transform/Transform3D.h"
#include "Engine/Transform/TransformSystem.h"

namespace engine
{

void Renderer2DSystem::Update()
{
	auto view = m_ECS_Manager.GetComponentView<Transform3D,Sprite2D>();

	engine::Renderer2D::BeginScene(m_orthoCam);
	for (auto it : view)
	{
		auto& transform = m_ECS_Manager.GetComponent<Transform3D>(it);
		auto& sprite = m_ECS_Manager.GetComponent<Sprite2D>(it);

		//Renderer2D::DrawQuad(transform.GetGlobalPosition(), transform.GetGlobalScale(), sprite.GetColor());
		Renderer2D::DrawRotatedQuad(transform.GetGlobalPosition(),
									transform.GetGlobalScale(),
									transform.GetGlobalRotationDeg(),
									sprite.GetTexture(),1.0f,
									sprite.GetColor());

		Renderer2D::DrawCircle(transform.GetGlobalPosition(),
							   transform.GetGlobalRotationDeg(),
							   transform.GetGlobalScale().x/2.f,
							   sprite.GetColor());
	}
	engine::Renderer2D::EndScene();
}

};
