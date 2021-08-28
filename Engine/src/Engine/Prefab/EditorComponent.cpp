#include "pch.h"
#include <rttr/registration>
#include "EditorComponent.h"
#include "EditorComponentSystem.h"
#include "Engine/Scene/SceneManager.h"

namespace engine
{

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
	m_isPrefab = false;
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(m_prefabReference,*this);
}

void EditorComponent::UpdatePrefab()
{
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UpdatedPrefab(*this);
}
engine::EditorComponent::EditorComponent(engine::Entity entity, bool active)
	:Component{ entity,active }
{
}
bool EditorComponent::IsPrefab()
{
	return m_isPrefab; 
}
bool EditorComponent::IsPrefabDirty()
{
	return m_isPrefab_Dirty;
}
engine::Entity EditorComponent::GetPrefabReference()
{
	return m_prefabReference; 
}
void EditorComponent::SetPrefabDirty(bool pd)
{

	m_isPrefab_Dirty = pd;
};
void EditorComponent::SetPrefabReference(engine::Entity e)
{ 
	if(m_prefabReference)
		engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(m_prefabReference,*this);
	m_prefabReference = e; 
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->RegisterNewUser(m_prefabReference, *this);
	m_isPrefab = true;
};

}