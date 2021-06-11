//#include "pch.h"
#include "Editor.h"

#include "../ImGui/imgui.h"
#include <filesystem>

#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include "../../vendor/ImGui/imgui_internal.h"

#include "Engine/Core/Input.h"

/* static vars */
testclass Editor::s_rootnode;//will be removed once ecs done
std::vector<testclass> Editor::s_testList;//will be removed once ecs done

std::map<KEY_ACTIONS, unsigned int> Editor::s_hotkeymapping;//will be shifted

//for copy and pasting
std::pair<std::string, std::shared_ptr<void*>> Editor::s_copyPayload = {"",nullptr};

Editor::Editor(const std::string& root)
{
	s_testList.reserve(50);

	s_hotkeymapping[KEY_ACTIONS::RENAME_ITEM] = 59;//f2
	s_hotkeymapping[KEY_ACTIONS::DELETE_ITEM] = 76;//del
	s_hotkeymapping[KEY_ACTIONS::HIDE_INSPECTOR] = 30;//1
	s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTHIRECHY] = 31;//2
	s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTFOLDERVIEW] = 32;//3
	s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTVIEW] = 33;//4

	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE);

}

Editor::~Editor()
{
}

void Editor::HotKeysUpdate()
{
	/*if(engine::Input::IsKeyPressed(engine::key::W))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	}*/

	if (ImGui::IsKeyPressed(s_hotkeymapping[KEY_ACTIONS::HIDE_INSPECTOR]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTHIRECHY]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTFOLDERVIEW]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(s_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTVIEW]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE);
	}
}
void Editor::UpdateUI()
{
}

void Editor::SaveData()
{
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();
	std::ofstream stream("inspector.json");
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	for (int i = 0; i < s_testList.size(); ++i)
	{
		writer.Key(std::to_string(s_testList[i].uid).c_str());
		writer.StartArray();

		writer.Key(s_testList[i].get_type().get_name().c_str());
		writer.StartArray();//start component 1
		for (int j = 0; j < list.size(); ++j)
		{
			if (list[j].get_type() == rttr::type::get<int>())
			{
				writer.Int(list[j].get_value(s_testList[i]).to_int());
			}
			else if (list[j].get_type() == rttr::type::get<std::string>())
			{
				writer.String(list[j].get_value(s_testList[i]).to_string().c_str());
			}

		}
		writer.EndArray();//end component 1
		writer.EndArray();
	}
	writer.EndObject();
}
void Editor::LoadData(const char* dir)
{
	std::ifstream ifs(dir);
	rapidjson::Document doc;
	rapidjson::IStreamWrapper isw(ifs);
	doc.ParseStream(isw);
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();
	int counter = 0;
	for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
	{
		std::cout << (*it).name.GetString() << std::endl;
		rapidjson::GenericArray arr = it->value.GetArray();
		for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
		{
			if (rttr::type::get_by_name(arr[i].GetString()).is_valid())
			{
				++i;
				rapidjson::GenericArray member = arr[i].GetArray();
				s_testList[counter].CreateTestClass(member[0].GetInt(), member[1].GetInt(), member[2].GetInt(), member[3].GetString());
			}
		}
		++counter;
	}
}

void Editor::TestFunction()
{
	//main banner
	ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::ShowDemoWindow();
	if(m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE))
	{
		m_inspector_view.Show();
	}

	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE))
	{
		m_heirarchy_view.Show();
	}


	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE))
	{
		m_projectroot_view.Show();
	}

	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE))
	{
		m_projectfolder_view.Show();
	}
	m_logging_view.Show();
	HotKeysUpdate();
}
