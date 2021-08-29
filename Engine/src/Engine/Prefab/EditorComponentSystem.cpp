#include "pch.h"

//base components
#include "Engine/ECS/ECS_Manager.h"
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/ECS_Utility.h"
#include "Engine/Transform/Transform3D.h"
//logging
#include "Engine/Core/Log.h"
//external components
#include "Engine/Scene/SceneManager.h"
#include "EditorComponentSystem.h"
#include "PrefabComponentSystem.h"
#include "EditorComponent.h"
#include "Seralizer.h"
namespace engine
{

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
	engine::GameObject& Prefab = (engine::GameObject)id;
	object.SetPrefabDirty(false);
	GameObject& go = (GameObject)object.GetEntity();//remove later
	engine::Transform3D& prefabTransform = Prefab.Transform();
	Prefab.CopyComponent<Transform3D>(go);

	//update users
	for (auto ent : iter->second)
	{
		engine::GameObject& gameobject = (engine::GameObject)ent;
		gameobject.CopyComponent<Transform3D>(Prefab);
	}
	//updating prefab to file
	engine::SceneManager::GetActiveWorld().GetSystem<engine::PrefabComponentSystem>()->SavePrefab(Prefab);
}

void EditorComponentSystem::RegisterNewUser(Entity prefab,value_reference object)
{
	m_prefabUsers[prefab].emplace_back(object.GetEntity());
}

void EditorComponentSystem::UnregisterUser(Entity prefab,value_reference object)
{
	Entity head = object.GetHeadReference();
	GameObject& headGo = static_cast<GameObject>(head);
	value_reference headRef = headGo.GetComponent<value_type>();
	RemoveUsers(headRef.GetPrefabReference(), headRef);
	auto& childList = headGo.GetChildren();
	for (GameObject child: childList)
	{
		value_reference childRef = child.GetComponent<value_type>();
		RemoveUsers(childRef.GetPrefabReference(), childRef);
	}
}

void EditorComponentSystem::RemoveUsers(Entity prefab , value_reference object)
{
	auto& iter = m_prefabUsers.find(prefab);
	if (iter == m_prefabUsers.end())
	{
		LOG_ERROR("Find User Failed.");
		return;
	}
	auto& vector_reference = iter->second;
	auto& target = std::find(vector_reference.begin(), vector_reference.end(), object.GetEntity());
	std::swap(*target, vector_reference.back());
	vector_reference.pop_back();
	object.SetIsPrefab(false);
}

}