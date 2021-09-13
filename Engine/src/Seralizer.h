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
#include "Engine/Scene/Scene.h"
class Serializer
{
public:


	using SaveComponentCallback = std::function<void(rttr::variant&, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&)> ;
	using LoadComponentCallback = std::function<void(rapidjson::Value::ValueType&,rttr::property&, rttr::instance&)>;
	using LoadGameObjectCallback = std::function<void(rapidjson::Value::Array&, engine::GameObject&)>;

	using ValueType = std::pair<rttr::type::type_id, SaveComponentCallback>;
	using LoadType = std::pair<rttr::type::type_id, LoadComponentCallback>;
	using LoadGOType = std::pair<engine::utility::StringHash::size_type, LoadGameObjectCallback>;



	Serializer();
	~Serializer() {};
	/*********************************************************************************//*!
	\brief    Load object usually used for prefabs
	 
	\param    prefab prefab filename
	\param    parent the head to attatch to
	\return   the parent
	*//**********************************************************************************/
	static engine::Entity LoadObject(const std::string& prefab,engine::Entity parent = 0);
	/*********************************************************************************//*!
	\brief    save prefab
	 
	\param    prefab filepath location
	\param    object start of item to save
	
	*//**********************************************************************************/
	static void SaveObject(const std::string& prefab,engine::Entity object);
	/*********************************************************************************//*!
	\brief    Save the whole scene
	 
	\param    path - directory to save to
	\param    scene - the scene file else active scene
	
	*//**********************************************************************************/
	static void SaveWorld(const std::string& path,const engine::Scene& scene);
	static void SaveWorld(const std::string& path);

	/*********************************************************************************//*!
	\brief    Load scene
	 
	\param    path - path to load from
	\param    scene - load into scene / else use active scene
	
	*//**********************************************************************************/
	static void LoadWorld(const std::string& path,const engine::Scene& scene);
	static void LoadWorld(const std::string& path);
private:
	/*********************************************************************************//*!
	\brief    Save hierarchy of object
	 
	\param    go - start of object
	\param    osw - write to file
	
	*//**********************************************************************************/
	static void SaveHierarchy(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& osw);
	/*********************************************************************************//*!
	\brief    Save object

	\param    go - start of object
	\param    osw - write to file

	*//**********************************************************************************/
	static void SaveItem(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& osw);
	
	
	/*********************************************************************************//*!
	\brief    Saves individual component using rttr to reference
	 
	\param    component - component to save
	\param    writer - write to file
	
	*//**********************************************************************************/
	template<typename Component>
	static void SaveComponent(Component & component, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	/*********************************************************************************//*!
	\brief    load component from file using the declared rttr format
	 
	\param    arr - rapidjson object containing data
	\param    component - component to populate
	
	*//**********************************************************************************/
	template<typename Component>
	static void LoadComponent(rapidjson::Value::Array& arr, Component& component);

	/*********************************************************************************//*!
	\brief    saving scripting components into file
	 
	\param    go - object containing the scripting component
	\param    writer - write to file
	
	*//**********************************************************************************/
	static void SaveScripts(engine::GameObject& go, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	/*********************************************************************************//*!
	\brief    Load script's variables from file 
	 
	\param    arr - rapidjson data
	\param    go - target to write scipting data to
	
	*//**********************************************************************************/
	static void LoadScripts(rapidjson::Value::Array& arr, engine::GameObject& go);

	/**!*******************Helper functions for seralizing *********************************/
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
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
	writer.String(temp.c_str(),temp.size());
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
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
	rttr::instance var(component);
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
}
