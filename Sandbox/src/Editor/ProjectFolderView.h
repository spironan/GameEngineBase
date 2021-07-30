/*****************************************************************//**
 * \file   ProjectFolderView.h
 * \brief  Display the items inside the selected folder from
 *		   ProjectRootView
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <filesystem>
#include <string>
#include <vector>
#include <rttr/type.h>
#include <rttr/property.h>

#include "RttrTypeID.h"
#include "Engine/ECS/GameObject.h"


class ProjectFolderView
{
public:
	void Show();

private:
	void ProjectView();
	void SaveHierarchy(engine::GameObject& , rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& );
	void SaveObject(engine::GameObject&, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&);
	template <typename Component>
	void SaveComponent(const Component&, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&);
	void PathDir(std::filesystem::path&, std::string&);
	void SearchFilter();
private:
	std::string m_filter;
	bool m_filtering = false;


};


template<typename Component>
void ProjectFolderView::SaveComponent(const Component& component, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	std::string temp = rttr::type::get<Component>().get_name();
	writer.String(temp.c_str());
	writer.StartArray();
	std::vector<rttr::property> list = rttr::type::get<Component>().get_properties();
	rttr::type::type_id id;
	for (auto element :list)
	{
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
			glm::vec3 value = element.get_value(component).get_value<glm::vec3>();
			writer.Double(value.x);
			writer.Double(value.y);
			writer.Double(value.z);
			writer.EndArray();
		}
		else if (id == rttr_type_ID::m_tracked_ids[rttr_type_ID::type_MAT4])
		{
			writer.StartArray();
			glm::mat4 value = element.get_value(component).get_value<glm::mat4>();
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
