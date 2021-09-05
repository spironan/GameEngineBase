#include "pch.h"
#include "PrefabComponentSystem.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/Prefab/EditorComponentSystem.h"
#include "Utility/Hash.h"
#include "Seralizer.h"
#include "Engine/Scripting/Scripting.h"
//components
#include "PrefabComponent.h"
#include "EditorComponent.h"
namespace engine
{
void PrefabComponentSystem::SavePrefab()
{
	//currently just forcefully save all prefabs
	for (auto& iter : m_prefabDetails)
	{
		Serializer::SaveObject(iter.second.filename);
	}
}

Entity PrefabComponentSystem::AddPrefab(const std::string& filepath)
{
    utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	auto& iter = m_prefabDetails.find(fileHash);
	if (iter != m_prefabDetails.end())
		return iter->second.head;

	//use serializer to serialize prefab
	Entity headNode = Serializer::LoadObject(filepath, 500);//not shown in hierarchy
	ENGINE_ASSERT(headNode);//means seralization failed

	static_cast<GameObject>(headNode).GetComponent<PrefabComponent>().m_RootNode = true;
	m_prefabDetails[fileHash] = FileDetails{ headNode,filepath };

	return headNode;
}

Entity PrefabComponentSystem::GetPrefab(const std::string& filepath)
{
	utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	auto& item =  m_prefabDetails.find(fileHash);
	if (item != m_prefabDetails.end())
		return 0;

	return item->second.head;
}

void PrefabComponentSystem::InstantiateFromPrefab(const std::string& filepath, GameObject& head)
{
	Entity obj = AddPrefab(filepath);
	GameObject& GO = static_cast<GameObject>(obj);
	Transform3D& trans = GO.GetComponent<Transform3D>();
	
	Transform3D& headTrans = head.GetComponent<Transform3D>();
	
	engine::SceneManager::GetActiveWorld().DuplicateEntity(GO, head);
	
	auto& childList = GO.GetChildren();
	std::vector<Entity> orignal{GO};
	std::vector<Entity> current{head};
	Entity prevParent = head;

	auto& headEditorComponent = head.EnsureComponent<EditorComponent>();

	headEditorComponent.SetPrefabReference(obj,head);

	for (GameObject childs : childList)
	{
		engine::GameObject		child = engine::SceneManager::GetActiveScene().CreateGameObject();
		engine::GameObject& copyObject = childs;
		engine::Transform3D& copyTransform = copyObject.GetComponent<engine::Transform3D>();
		//gameobject component
		child.Name() = copyObject.Name();
		child.Active() = static_cast<bool>(copyObject.Active());

		auto& editorComponent = child.AddComponent<EditorComponent>();
		editorComponent.SetPrefabReference(copyObject.GetEntity(),head);
		

		{//TODO fix this once its done
			engine::SceneManager::GetActiveWorld().DuplicateEntity(copyObject, child);
		}

		const engine::Entity parentid = copyTransform.GetParentId();

		{
			auto& iter = std::find(orignal.begin(), orignal.end(), parentid);
			if (iter != orignal.end())//means there is a result
			{
				orignal.erase(++iter, orignal.end());
				current.resize(orignal.size());
				prevParent = current.back();
			}
			else
			{
				prevParent = head;
			}
		}

		static_cast<engine::GameObject>(prevParent).AddChild(child);
		if (copyTransform.GetChildCount())
		{
			orignal.emplace_back(copyTransform.GetEntity());
			current.emplace_back(child.GetEntity());
			prevParent = child;
		}
	}

}

void PrefabComponentSystem::MakePrefab(const std::string& filepath, GameObject& head)
{
	Serializer::SaveObject(filepath);
	GameObject prefab = AddPrefab(filepath);

	auto& prefabChild = prefab.GetChildren();
	auto& childList = head.GetChildren();
	
	if (head.HasComponent<EditorComponent>())
	{
		EditorComponent& ec = head.GetComponent<EditorComponent>();
		ec.SetPrefabReference(prefab.GetEntity(),head);
	}

	for (size_t iter = 0; iter < childList.size(); ++iter)
	{
		EditorComponent& ec = static_cast<GameObject>(childList[iter]).GetComponent<EditorComponent>();
		ec.SetPrefabReference(prefabChild[iter],head);
	}

}

void PrefabComponentSystem::SavePrefab(GameObject& go)
{

	for (auto& element : m_prefabDetails)
	{
		if (element.second.head == go)
			Serializer::SaveObject(element.second.filename);
	}
}

}
