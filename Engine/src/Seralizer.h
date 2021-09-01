#pragma once
#include <string>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/document.h>

#include "Engine/ECS/GameObject.h"
class Serializer
{
public:
	Serializer() {};
	~Serializer() {};
	static engine::Entity LoadObject(const std::string& prefab,engine::Entity parent = 0);
	static void SaveObject(const std::string& prefab);
	static void SaveWorld(const std::string& path);
	static void LoadWorld(const std::string& path);
private:
	static void SaveHierarchy(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& osw);
	static void SaveItem(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& osw);
	template<typename Component>
	static void SaveComponent(Component & component, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	static void LoadComponent(rapidjson::Value::Array& arr,engine::GameObject& go);

	static oom::vec3 GetVec3(rapidjson::Value& val)
	{
		auto& arr = val.GetArray();
		return { arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat() };
	}
};

template<typename Component>
void Serializer::SaveComponent(Component& component, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	std::string temp = rttr::type::get<Component>().get_name();
	writer.String(temp.c_str());
	writer.StartArray();
	std::vector<rttr::property> list = rttr::type::get<Component>().get_properties();
	rttr::type::type_id id;
	for (auto& element : list)
	{
		if (element.is_readonly())
			continue;
		id = element.get_type().get_id();
		if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_INT])
		{
			writer.Int(element.get_value(component).get_value<int>());
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_FLOAT])
		{
			writer.Double(element.get_value(component).get_value<float>());
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_STRING])
		{
			writer.String(element.get_value(component).get_value<std::string>().c_str());
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_BOOL])
		{
			writer.Bool(element.get_value(component).get_value<bool>());
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_ENTITY])
		{
			writer.Uint(element.get_value(component).get_value<engine::Entity>());
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_VEC3])
		{
			writer.StartArray();
			oom::vec3 value = element.get_value(component).get_value<oom::vec3>();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.Double(value.z);
			writer.EndArray();
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_MAT4])
		{
			writer.StartArray();
			oom::mat4 value = element.get_value(component).get_value<oom::mat4>();
			writer.Double(value[0].x);
			writer.Double(value[0].y);
			writer.Double(value[0].z);
			writer.Double(value[0].w);
			writer.Double(value[1].x);
			writer.Double(value[1].y);
			writer.Double(value[1].z);
			writer.Double(value[1].w);
			writer.Double(value[2].x);
			writer.Double(value[2].y);
			writer.Double(value[2].z);
			writer.Double(value[2].w);
			writer.Double(value[3].x);
			writer.Double(value[3].y);
			writer.Double(value[3].z);
			writer.Double(value[3].w);
			writer.EndArray();
		}
	}
	writer.EndArray();
}
