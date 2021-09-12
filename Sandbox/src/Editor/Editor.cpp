/*****************************************************************//**
 * \file   Editor.cpp
 * \brief  the implementation of all the 
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/

#include <filesystem>

#include <imgui.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include "Editor.h"
#include "EditorFileGroup.h"
#include "Engine/Core/Input.h"
#include "Engine/Memory/MemoryManager.h"

#include "imgui_internal.h"

/* static vars */

testclass Editor::s_rootnode;//will be removed once ecs done
std::vector<testclass> Editor::s_testList;//will be removed once ecs done

std::map<KEY_ACTIONS, unsigned int> Editor::s_hotkeymapping;//will be shifted



//for copy and pasting
std::pair<std::string, void* > Editor::s_copyPayload = {"",nullptr };

engine::BufferAllocator Editor::s_payloadBufferAllocator(engine::MemoryManager::NewBufferAllocator(2048, 8));

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

//setting the root path of the project view
#ifdef SANDBOX_PLATFORM_WINDOWS
	char exePath[1000];
	GetModuleFileNameA(NULL, exePath, 1000);
	FileGroup::ReassignRootPath(exePath);
#endif // SANDBOX_PLATFORM_WINDOWS
}

Editor::~Editor()
{
	//need to delete memory block of s_bufferAllocator
}

void Editor::MenuBarUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}
			if (ImGui::MenuItem("Open"))
			{

			}
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Reset Layout"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Logging", NULL, logging_widget))
			{
				logging_widget = !logging_widget;
			}
			if (ImGui::MenuItem("Action History", NULL, action_widget))
			{
				action_widget = !action_widget;
			}
			if (ImGui::MenuItem("Keyboard Settings", NULL, keyboardview_widget))
			{
				keyboardview_widget = !keyboardview_widget;
			}
			if (ImGui::MenuItem("Style Editor", NULL, styleeditor_widget))
			{
				styleeditor_widget = !styleeditor_widget;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void Editor::HotKeysUpdate()
{
	if(ImGui::GetActiveID())
		return;
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

/**
 * /brief
 *		this is the main function to update the widgets
 */
void Editor::ShowAllWidgets()
{
	//main banner
	ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::ShowDemoWindow();
	MenuBarUI();
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
	m_toolbar_view.Show();
	m_viewport_view.Show();
	m_assetbrowser.Show();
	//ImGui::ShowStyleEditor();
	FileGroup::ProjectViewPopUp();
	ActionStack::UpdateStack();
	HotKeysUpdate();

	m_warning_view.Show();
	if(keyboardview_widget)
		m_keyboard_view.Show(&keyboardview_widget);
	if(action_widget)
		m_action_stack.Show(&action_widget);
	if(logging_widget)
		m_logging_view.Show(&logging_widget);
	if(styleeditor_widget)
		m_styleEditor_view.Show(&styleeditor_widget);
}

