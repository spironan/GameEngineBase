#include "pch.h"
#include "Seralizer.h"

#include "../../Sandbox/src/Editor/RttrTypeID.h"
#include "../../Sandbox/src/Editor/EditorObjectGroup.h"


#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <istream>
#include <unordered_map>


#include "Engine/Scene/SceneManager.h"
#include "Engine/Transform/Transform3D.h"
#include "Engine/ECS/GameObject.h"
#include "Engine/Prefab/PrefabComponent.h"
#include "Engine/Prefab/EditorComponent.h"

engine::Entity Serializer::LoadObject(const std::string& prefab,engine::Entity parent)
{
	std::unordered_map<engine::Entity,std::pair<engine::Entity,engine::Entity>> hierarchymap;
	std::ifstream ifs;
	ifs.open(prefab);
	if (!ifs.is_open())
	{
		LOG_CRITICAL("LoadObject not able to open file");
		ENGINE_ASSERT(true);
	}
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	for (auto& iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
	{
		auto& arr = iter->value.GetArray();
		engine::GameObject object{ engine::GameObject::Create{} };
		hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetID(), arr[1].GetUint());//first element = parent id
		LoadComponent(arr, object);
		object.EnsureComponent<engine::PrefabComponent>();
	}
	engine::Entity head = 0;
	for (auto& hierarchyItem : hierarchymap)
	{
		if (hierarchymap.find(hierarchyItem.second.second) != hierarchymap.end())
		{
			engine::GameObject temp = static_cast<engine::GameObject>(hierarchymap[hierarchyItem.second.second].first);
			temp.AddChild(hierarchyItem.second.first);
		}
		else
		{
			head = hierarchyItem.second.first;
		}
	}
	ifs.close();
	return head;
}

void Serializer::SaveObject(const std::string& prefab)
{
	std::ofstream stream(prefab, std::ios::trunc);
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	SaveHierarchy(static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject), writer);
	writer.EndObject();
	stream.close();
}

void Serializer::SaveWorld(const std::string& path)
{
	std::ofstream stream(path,std::ios::trunc);
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	auto& list = engine::SceneManager::GetActiveScene().GetWorld().GetComponentDenseArray<engine::Transform3D>();
	for (size_t i = 1; i < list.size() ; ++i)//skip the root node
	{
		SaveItem(static_cast<engine::GameObject>(list[i].GetEntity()), writer);
	}
	writer.EndObject();
	stream.close();
}

void Serializer::LoadWorld(const std::string& path)
{
	//need to add the ability to preload the other world
	std::unordered_map<engine::Entity, std::pair<engine::Entity, engine::Entity>> hierarchymap;
	std::ifstream ifs;
	ifs.open(path);
	if (!ifs.is_open())
		return;
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	for (auto& iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
	{
		auto& arr = iter->value.GetArray();
		engine::GameObject object = engine::SceneManager::GetActiveScene().CreateGameObject();
		hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetID(), arr[1].GetUint());//first element = parent id
		LoadComponent(arr, object);
	}

	for (auto& hierarchyItem : hierarchymap)
	{
		if (hierarchymap.find(hierarchyItem.second.second) != hierarchymap.end())
		{
			engine::GameObject temp = static_cast<engine::GameObject>(hierarchymap[hierarchyItem.second.second].first);
			temp.AddChild(hierarchyItem.second.first);
		}
		else
		{
			//root
			engine::GameObject(engine::SceneManager::GetActiveRoot()).AddChild(hierarchyItem.second.first);
		}
	}
	ifs.close();
}

void Serializer::SaveHierarchy(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	auto& list = engine::SceneManager::GetActiveWorld().GetComponentDenseArray<engine::Transform3D>();
	engine::Entity id = go.GetID();
	int count = 0;

	auto& childrenList = go.GetChildren();
	SaveItem(go, writer); 
	for (engine::Entity gobj : childrenList)
	{
		SaveItem(static_cast<engine::GameObject>(gobj), writer);
	}
}
 
void Serializer::SaveItem(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	writer.Key(std::to_string(go.GetID()).c_str());
	writer.StartArray();
	writer.Uint(go.GetID());
	writer.Uint(go.GetComponent<engine::Transform3D>().GetParentId());//parent id first

	if (go.TryGetComponent<engine::GameObjectComponent>())
		SaveComponent<engine::GameObjectComponent>(go.GetComponent<engine::GameObjectComponent>(), writer);
	if (go.TryGetComponent<engine::Transform3D>())
		SaveComponent<engine::Transform3D>(go.GetComponent<engine::Transform3D>(), writer);
	
	writer.EndArray();
}

//this function will need constant updating
void Serializer::LoadComponent(rapidjson::Value::Array& arr,engine::GameObject& go)
{
	std::string component;
	for (rapidjson::SizeType count = 2; count < arr.Size(); ++count)
	{
		component = arr[count].GetString();
		++count;
		if (component == rttr::type::get<engine::GameObjectComponent>().get_name())
		{
			auto& component_data = arr[count].GetArray();
			engine::GameObjectComponent& goComponent = go.GetComponent<engine::GameObjectComponent>();
			goComponent.ActiveSelf = component_data[0].GetBool();
			goComponent.Name = component_data[1].GetString();
		}
		if (component == rttr::type::get<engine::Transform3D>().get_name())
		{
			auto& trans = arr[count].GetArray();
			engine::Transform3D& transform = go.GetComponent<engine::Transform3D>();

			transform.SetPosition(GetVec3(trans[0]));
			transform.SetRotationAxis(GetVec3(trans[1]));
			transform.SetRotationAngle(trans[2].GetFloat());
		}
		if (component == rttr::type::get<engine::PrefabComponent>().get_name())
		{
		}
	}
}
