#include "pch.h"
#include "Engine/ECS/ECS_Manager.h"
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/ECS_Utility.h"

#include "Engine/Core/Log.h"

#include "EditorComponentSystem.h"
#include "EditorComponent.h"

void EditorComponentSystem::Update()
{
	//checks if prefabs are dirty
}



void EditorComponentSystem::UpdatedPrefab(value_reference object)
{
	
	engine::Entity id = object.GetPrefabReference();
	auto& iter = m_prefabUsers.find(object.GetPrefabReference());
	if (iter == m_prefabUsers.end())
	{
		LOG_ERROR("Update Prefab Failed.");
		return;
	}
	//update prefab
	engine::GameObject& Prefab = (engine::GameObject)iter->first;
	//Prefab.CopyComponents(object.GetEntity());

	//update users
	for (auto ent : iter->second)
	{
		engine::GameObject& gameobject = (engine::GameObject)ent;
		//gameobject.CopyComponents(Prefab);
	}

}

void EditorComponentSystem::RegisterNewUser(value_reference object)
{
	auto& iter = m_prefabUsers.find(object.GetPrefabReference());
	if (iter == m_prefabUsers.end())
	{
		LOG_ERROR("RegisterNewUser Failed.");
		return;
	}
	iter->second.emplace_back(object.GetEntity());
}

void EditorComponentSystem::UnregisterUser(value_reference object)
{
	auto& iter = m_prefabUsers.find(object.GetPrefabReference());
	if (iter == m_prefabUsers.end())
	{
		LOG_ERROR("RegisterNewUser Failed.");
		return;
	}
	auto& vector_reference = iter->second;
	auto& target = std::find(vector_reference.begin(), vector_reference.end(), object.GetEntity());
	std::swap(*target, vector_reference.back());
	vector_reference.pop_back();
}
