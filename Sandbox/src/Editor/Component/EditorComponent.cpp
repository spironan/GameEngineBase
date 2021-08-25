#pragma once
#include <rttr/registration>
#include "EditorComponent.h"
#include "EditorComponentSystem.h"
#include "Engine/Scene/SceneManager.h"
RTTR_REGISTRATION
{
	using namespace rttr;
registration::class_<EditorComponent>("Editor Component")
	.property_readonly("Is Prefab",&EditorComponent::IsPrefab)
	.property_readonly("Is Dirty" ,&EditorComponent::IsPrefabDirty)
	.property_readonly("Prefab Reference",&EditorComponent::GetPrefabReference);
}

void EditorComponent::BreakOffFromPrefab()
{
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(*this);
}

void EditorComponent::UpdatePrefab()
{
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UpdatedPrefab(*this);
}
