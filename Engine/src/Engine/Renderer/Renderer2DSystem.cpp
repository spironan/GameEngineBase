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
		
		Renderer2D::DrawQuad(transform.GetPosition(), glm::vec2{ transform.GetScale() }, sprite.GetColor() );
	}
	engine::Renderer2D::EndScene();
}

};
