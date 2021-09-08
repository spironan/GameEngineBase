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
#include "Engine/Scripting/Scripting.h"
#include "Engine/Scripting/ScriptSystem.h"


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
		hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetEntity(), arr[1].GetUint());//first element = parent id
		LoadComponent(arr, object);
		object.EnsureComponent<engine::PrefabComponent>();
		object.EnsureComponent<engine::EditorComponent>().SetShownInEditor(false);
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
		hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetEntity(), arr[1].GetUint());//first element = parent id
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
	engine::Entity id = go.GetEntity();
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
	writer.Key(std::to_string(go.GetEntity()).c_str());
	writer.StartArray();
	writer.Uint(go.GetEntity());
	writer.Uint(go.GetComponent<engine::Transform3D>().GetParentId());//parent id first

	if (go.HasComponent<engine::GameObjectComponent>())
		SaveComponent<engine::GameObjectComponent>(go.GetComponent<engine::GameObjectComponent>(), writer);
	if (go.HasComponent<engine::Transform3D>())
		SaveComponent<engine::Transform3D>(go.GetComponent<engine::Transform3D>(), writer);
	
	if (go.HasComponent<engine::Scripting>())
		SaveScripts(go, writer);
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
			goComponent.Active = component_data[0].GetBool();
			goComponent.Name = component_data[1].GetString();
		}
		else if (component == rttr::type::get<engine::Transform3D>().get_name())
		{
			auto& trans = arr[count].GetArray();
			engine::Transform3D& transform = go.GetComponent<engine::Transform3D>();

			transform.SetPosition(GetVec3(trans[0]));
			transform.SetRotationAxis(GetVec3(trans[1]));
			transform.SetRotationAngle(trans[2].GetFloat());
			
		}
		else if (component == rttr::type::get<engine::PrefabComponent>().get_name())
		{

		}
		else if (component == "Scripting")
		{
			LoadScripts(arr[count].GetArray(), go);
		}
	}
}

void Serializer::SaveScripts(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	auto& scriptInfoList = go.GetComponent<engine::Scripting>().GetScriptInfoAll();
	std::string temp = "Scripting";
	writer.String(temp.c_str(),temp.size());
	writer.StartArray();
	
	for (auto& script : scriptInfoList)
	{
		temp = script.second.classInfo.ToString();
		writer.String(temp.c_str(), temp.size());
		writer.StartArray();
		for (auto& values : script.second.fieldMap)
		{
			engine::ScriptFieldValue& sfValue = values.second.value;
			switch (sfValue.GetValueType())
			{
				case engine::ScriptValueType::BOOL:
				{
					writer.Bool(sfValue.GetValue<bool>());
					break;
				}
				case engine::ScriptValueType::INT:
				{
					writer.Int(sfValue.GetValue<int>());
					break;
				}
				case engine::ScriptValueType::FLOAT:
				{
					writer.Double(sfValue.GetValue<float>());
					break;
				}
				case engine::ScriptValueType::STRING:
				{
					temp = sfValue.GetValue<std::string>();
					writer.String(temp.c_str(), temp.size());
					break;
				}
				case engine::ScriptValueType::GAMEOBJECT:
				{
					writer.Uint(sfValue.GetValue<engine::Entity>());
					break;
				}
			}
		}
		writer.EndArray();
	}
	writer.EndArray();
}


void Serializer::LoadScripts(rapidjson::Value::Array& arr, engine::GameObject& go)
{
	std::string componentName;
	engine::Scripting& scriptComponent = go.EnsureComponent<engine::Scripting>();
	for (rapidjson::SizeType iter = 0; iter < arr.Size(); ++iter)
	{
		componentName = arr[iter].GetString();
		size_t pos = componentName.find_last_of('.');
		pos = (pos == std::string::npos) ? 0 : pos;
		engine::ScriptInfo& info = scriptComponent.AddScriptInfo({ componentName.substr(0, pos), componentName.substr(pos) });
		
		++iter;//added component
		//populate data
		auto & componentData = arr[iter].GetArray();
		size_t component_iter = 0;
		for (auto& field : info.fieldMap)
		{
			engine::ScriptFieldValue& sfValue = field.second.value;
			switch (sfValue.GetValueType())
			{
			case engine::ScriptValueType::BOOL:
			{
				sfValue.SetValue<bool>(componentData[component_iter].GetBool());
				break;
			}
			case engine::ScriptValueType::INT:
			{
				sfValue.SetValue<int>(componentData[component_iter].GetInt());
				break;
			}
			case engine::ScriptValueType::FLOAT:
			{
				sfValue.SetValue<float>(componentData[component_iter].GetFloat());
				break;
			}
			case engine::ScriptValueType::STRING:
			{
				sfValue.SetValue<std::string>(componentData[component_iter].GetString());
				
				break;
			}
			case engine::ScriptValueType::GAMEOBJECT:
			{
				break;
			}
			}
			++component_iter;
		}
	}
}