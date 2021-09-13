#include "pch.h"
#include "Engine.h"

#include "../../Sandbox/src/Editor/RttrTypeID.h"
#include "../../Sandbox/src/Editor/EditorObjectGroup.h"


#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <istream>
#include <unordered_map>
#include <vector>

#include "Seralizer.h"


#define  LOAD_OBJECT(T)	LoadGOType\
	{\
	engine::utility::StringHash(rttr::type::get<T>().get_name()),\
		[](rapidjson::Value::Array& arr, engine::GameObject& go)\
	{\
		T& var = go.EnsureComponent<T>();\
		LoadComponent<T>(arr, var);\
	}\
	}\

std::map<rttr::type::type_id, Serializer::SaveComponentCallback> Serializer::m_SaveComponentCallbacks
{
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_INT],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{writer.Int(variant.get_value<int>()); }
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_FLOAT],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{writer.Double(variant.get_value<float>()); }
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_STRING],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			std::string temp = variant.get_value<std::string>();
			writer.String(temp.c_str(), static_cast<rapidjson::SizeType>(temp.size()));
		}
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_BOOL],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			writer.Bool(variant.get_value<bool>()); 
		}
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_ENTITY],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{writer.Uint(variant.get_value<engine::Entity>()); }
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_VEC2],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			writer.StartArray();
			oom::vec2 value = variant.get_value<oom::vec2>();
			writer.Double(value.x);writer.Double(value.y);
			writer.EndArray();
		}
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_VEC3],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			writer.StartArray();
			oom::vec3 value = variant.get_value<oom::vec3>();
			writer.Double(value.x);writer.Double(value.y);writer.Double(value.z);
			writer.EndArray();
		}
	},
	ValueType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_MAT4],
		[](rttr::variant& variant, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			writer.StartArray();
			oom::mat4 value = variant.get_value<oom::mat4>();
			writer.Double(value[0].x);writer.Double(value[0].y);writer.Double(value[0].z);writer.Double(value[0].w);
			writer.Double(value[1].x);writer.Double(value[1].y);writer.Double(value[1].z);writer.Double(value[1].w);
			writer.Double(value[2].x);writer.Double(value[2].y);writer.Double(value[2].z);writer.Double(value[2].w);
			writer.Double(value[3].x);writer.Double(value[3].y);writer.Double(value[3].z);writer.Double(value[3].w);
			writer.EndArray();
		}
	}
};

std::map<rttr::type::type_id, Serializer::LoadComponentCallback> Serializer::m_LoadComponentCallbacks
{
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_INT],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{prop.set_value(variant,data.GetInt());}
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_FLOAT],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{prop.set_value(variant,data.GetFloat()); }
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_STRING],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{prop.set_value(variant,data.GetString()); }
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_BOOL],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{prop.set_value(variant,data.GetBool()); }
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_ENTITY],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{prop.set_value(variant,data.GetUint()); }
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_VEC2],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{
			auto& arr = data.GetArray();
			oom::vec2 vector = { arr[0].GetFloat(),arr[1].GetFloat() };
			prop.set_value(variant,vector); 
		}
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_VEC3],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{
			auto& arr = data.GetArray();
			oom::vec3 vector = { arr[0].GetFloat(),arr[1].GetFloat(),arr[2].GetFloat() };
			prop.set_value(variant,vector);
		}
	},
	LoadType
	{
		rttr_type_ID::m_tracked_ids[rttr_type_ID::type_MAT4],
		[](rapidjson::Value::ValueType& data,rttr::property& prop, rttr::instance& variant)
		{
			auto& arr = data.GetArray();
			oom::mat4 mat = {	arr[0].GetFloat(),arr[1].GetFloat(),arr[2].GetFloat(),arr[3].GetFloat(),
								arr[4].GetFloat(),arr[5].GetFloat(),arr[6].GetFloat(),arr[7].GetFloat(), 
								arr[8].GetFloat(),arr[9].GetFloat(),arr[10].GetFloat(),arr[11].GetFloat(), 
								arr[12].GetFloat(),arr[13].GetFloat(),arr[14].GetFloat(),arr[15].GetFloat() 
							};
			prop.set_value(variant, mat);
		}
	}
};
std::map<engine::utility::StringHash::size_type, Serializer::LoadGameObjectCallback> Serializer::m_LoadGameObjectCallbacks;


Serializer::Serializer()
{
	m_LoadGameObjectCallbacks = 
	{
		LOAD_OBJECT(engine::GameObjectComponent),
		LOAD_OBJECT(engine::Transform3D),
		LOAD_OBJECT(engine::Collider2D),
		LOAD_OBJECT(engine::CircleCollider2D),
		LOAD_OBJECT(engine::BoxCollider2D),
		LOAD_OBJECT(engine::Sprite2D),
		LOAD_OBJECT(engine::EditorComponent),
		LOAD_OBJECT(engine::SceneCamera),
		LoadGOType
		{
		engine::utility::StringHash(rttr::type::get<engine::Scripting>().get_name()),
			[](rapidjson::Value::Array& arr, engine::GameObject& go)
			{
				LoadScripts(arr,go);
			}
		}
	};
}

engine::Entity Serializer::LoadObject(const std::string& prefab,engine::Entity parent)
{
	
	std::vector<engine::Entity> entList;
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
		entList.emplace_back(object);
		object.GetComponent<engine::Transform3D>().SetChildCount(arr[1].GetInt());
		for (rapidjson::SizeType counter = 2; counter < arr.Size(); ++counter)
		{
			std::string temp = arr[counter].GetString();
			auto& iter = m_LoadGameObjectCallbacks.find(engine::utility::StringHash(temp));
			if ((temp) == (rttr::type::get<engine::SceneCamera>().get_name()))
				std::cout << engine::utility::StringHash(temp) << std::endl;
			++counter;
			if(iter != m_LoadGameObjectCallbacks.end())
				iter->second(arr[counter].GetArray(),object);
		}

		object.EnsureComponent<engine::PrefabComponent>();
		object.EnsureComponent<engine::EditorComponent>().SetShownInEditor(true);//for now set it to true for easier debug
	}
	engine::SceneManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->UseDenseArrayAsHierarchy();

	ifs.close();
	return entList.front();
}

void Serializer::SaveObject(const std::string& prefab, engine::Entity object)
{
	std::ofstream stream(prefab, std::ios::trunc);
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	SaveHierarchy(static_cast<engine::GameObject>(object), writer);
	writer.EndObject();
	stream.close();
}
void Serializer::SaveWorld(const std::string& path, const engine::Scene& scene)
{
	std::ofstream stream(path,std::ios::trunc);
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	auto& list = scene.GetWorld().GetComponentDenseArray<engine::Transform3D>();
	for (size_t i = 1; i < list.size() ; ++i)//skip the root node
	{
		SaveItem(static_cast<engine::GameObject>(list[i].GetEntity()), writer);
	}
	writer.EndObject();
	stream.close();
}
void Serializer::SaveWorld(const std::string& path)
{
	std::ofstream stream(path, std::ios::trunc);
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	auto& list = engine::SceneManager::GetActiveScene().GetWorld().GetComponentDenseArray<engine::Transform3D>();
	for (size_t i = 1; i < list.size(); ++i)//skip the root node
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
		//hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetEntity(), arr[1].GetUint());//first element = parent id
		object.GetComponent<engine::Transform3D>().SetChildCount(arr[1].GetInt());

		for (rapidjson::SizeType counter = 2; arr.Size(); ++counter)
		{
			m_LoadGameObjectCallbacks
				[engine::utility::StringHash(arr[counter].GetString())]
			(arr[++counter].GetArray(), object);
		}
	}
	engine::SceneManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->UseDenseArrayAsHierarchy();

	ifs.close();
}
void Serializer::LoadWorld(const std::string& path, const engine::Scene& scene)
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
		engine::GameObject object = scene.CreateGameObject();
		//hierarchymap[arr[0].GetUint()] = std::pair<engine::Entity, engine::Entity>(object.GetEntity(), arr[1].GetUint());//first element = parent id
		object.GetComponent<engine::Transform3D>().SetChildCount(arr[1].GetInt());
		for (rapidjson::SizeType counter = 2; counter < arr.Size(); ++counter)
		{
			std::string temp = arr[counter].GetString();
			auto& iter = m_LoadGameObjectCallbacks.find(engine::utility::StringHash(temp));
			if ((temp) == (rttr::type::get<engine::SceneCamera>().get_name()))
				std::cout << engine::utility::StringHash(temp) << std::endl;
			++counter;
			if (iter != m_LoadGameObjectCallbacks.end())
				iter->second(arr[counter].GetArray(), object);
		}
	}
	engine::SceneManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->UseDenseArrayAsHierarchy();
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
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	writer.StartArray();
	writer.Uint(go.GetEntity());
	writer.Int(go.GetComponent<engine::Transform3D>().GetChildCount());//parent id first

	if (go.HasComponent<engine::GameObjectComponent>())
		SaveComponent<engine::GameObjectComponent>(go.GetComponent<engine::GameObjectComponent>(), writer);
	if (go.HasComponent<engine::Transform3D>())
		SaveComponent<engine::Transform3D>(go.GetComponent<engine::Transform3D>(), writer);
	if (go.HasComponent<engine::Camera>())
		SaveComponent<engine::Camera>(go.GetComponent<engine::Camera>(), writer);
	if (go.HasComponent<engine::SceneCamera>())
		SaveComponent<engine::SceneCamera>(go.GetComponent<engine::SceneCamera>(), writer);
	if (go.HasComponent<engine::EditorComponent>())
		SaveComponent<engine::EditorComponent>(go.GetComponent<engine::EditorComponent>(), writer);



	if (go.HasComponent<engine::Scripting>())
		SaveScripts(go, writer);
	writer.EndArray();
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
}

void Serializer::SaveScripts(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
	auto& scriptInfoList = go.GetComponent<engine::Scripting>().GetScriptInfoAll();
	std::string temp = "Scripting";
	writer.String(temp.c_str(), static_cast<rapidjson::SizeType>(temp.size()));
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	writer.StartArray();
	
	for (auto& script : scriptInfoList)
	{
		temp = script.second.classInfo.ToString();
		writer.String(temp.c_str(), static_cast<rapidjson::SizeType>(temp.size()));
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
					writer.String(temp.c_str(), static_cast<rapidjson::SizeType>(temp.size()));
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
				sfValue.SetValue<bool>(componentData[static_cast<rapidjson::SizeType>(component_iter)].GetBool());
				break;
			}
			case engine::ScriptValueType::INT:
			{
				sfValue.SetValue<int>(componentData[static_cast<rapidjson::SizeType>(component_iter)].GetInt());
				break;
			}
			case engine::ScriptValueType::FLOAT:
			{
				sfValue.SetValue<float>(componentData[static_cast<rapidjson::SizeType>(component_iter)].GetFloat());
				break;
			}
			case engine::ScriptValueType::STRING:
			{
				sfValue.SetValue<std::string>(componentData[static_cast<rapidjson::SizeType>(component_iter)].GetString());
				
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