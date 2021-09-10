#pragma once
#include <string>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/document.h>
#include <map>
#include <functional>
#include "Utility/Hash.h"
#include "Engine/ECS/GameObject.h"

#include "rttr/type.h"
#include "rttr/property.h"
#include "rttr/variant.h"
class Serializer
{
public:


	using SaveComponentCallback = std::function<void(rttr::variant&, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&)> ;
	using LoadComponentCallback = std::function<void(rapidjson::Value::ValueType&,rttr::property&, rttr::variant&)>;
	using LoadGameObjectCallback = std::function<void(rapidjson::Value::Array&, engine::GameObject&)>;

	using ValueType = std::pair<rttr::type::type_id, SaveComponentCallback>;
	using LoadType = std::pair<rttr::type::type_id, LoadComponentCallback>;
	using LoadGOType = std::pair<engine::utility::StringHash::size_type, LoadGameObjectCallback>;



	Serializer();
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
	template<typename Component>
	static void LoadComponent(rapidjson::Value::Array& arr, Component& component);

	static void SaveScripts(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	static void LoadScripts(rapidjson::Value::Array& arr, engine::GameObject& go);


	static oom::vec2 GetVec2(rapidjson::Value& val)
	{
		auto& arr = val.GetArray();
		return {arr[0].GetFloat(), arr[1].GetFloat()};
	}
	static oom::vec3 GetVec3(rapidjson::Value& val)
	{
		auto& arr = val.GetArray();
		return { arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat() };
	}

private:
	static std::map<rttr::type::type_id, SaveComponentCallback> m_SaveComponentCallbacks;
	static std::map<rttr::type::type_id, LoadComponentCallback> m_LoadComponentCallbacks;
	static std::map<engine::utility::StringHash::size_type, LoadGameObjectCallback> m_LoadGameObjectCallbacks;
};

template<typename Component>
void Serializer::SaveComponent(Component& component, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	std::string temp = rttr::type::get<Component>().get_name();
	writer.String(temp.c_str(),temp.size());
	writer.StartArray();
	std::vector<rttr::property> list = rttr::type::get<Component>().get_properties();
	rttr::type::type_id id;
	for (auto& element : list)
	{
		if (element.is_readonly())
			continue;
		rttr::variant comp = component;
		id = element.get_type().get_id();
		auto& iter = m_SaveComponentCallbacks.find(id);
		if(iter != m_SaveComponentCallbacks.end())
			iter->second(element.get_value(comp), writer);
	}
	writer.EndArray();
}
template<typename Component>
void Serializer::LoadComponent(rapidjson::Value::Array& arr, Component& component)
{
	rttr::type type = rttr::type::get<Component>();
	auto& properties = type.get_properties();
	rapidjson::SizeType counter = 0;
	rttr::variant var = component;
	for (auto& element : properties)
	{
		if (element.is_readonly())
			continue;
		auto& iter = m_LoadComponentCallbacks.find(element.get_type().get_id());
		if (iter == m_LoadComponentCallbacks.end())
			continue;
		iter->second(arr[counter], element, var);
		std::cout << typeid(Component).name() <<counter << std::endl;
		++counter;
	}
	component = var.get_value<Component>();
}
