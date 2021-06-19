/*****************************************************************//**
 * \file   EditorFileGroup.cpp
 * \brief  Contains the popup that is shared between both root view
 *		   and folderview
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#include "EditorFileGroup.h"
#include "Editor.h"

#include <imgui_internal.h>
#include <filesystem>

#include <windows.h>

std::string FileGroup::s_rootPath = "C:/Users/User/Desktop/New folder (3)";
std::string FileGroup::s_CurrentPath = s_rootPath;
std::string FileGroup::s_hoveredPath = s_rootPath;
std::string FileGroup::s_selectedpath;//path with the itemname
std::string FileGroup::s_selecteditem;//itemname only
ImVec2 FileGroup::s_selectedItemPosition;//vec2
char FileGroup::s_nameBuffer[128] = "";//for renaming items

ImGuiID FileGroup::s_projectviewid = 100;

bool FileGroup::s_delete_popup = false;//flag for deleting modal popup

bool FileGroup::s_rename_item = false;


/**
 * \brief this code will be activated when right clicking inside the 
 *			folder view or rootview windows
 * \note  This function should not be called by anyone other than
 *		  the main display function
 */
void FileGroup::ProjectViewPopUp()
{

	if (ImGui::BeginPopupEx(FileGroup::s_projectviewid, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings))
	{
		ProjectViewPopUpOptions();
		ImGui::EndPopup();
		if (FileGroup::s_delete_popup)
		{
			FileGroup::s_delete_popup = false;
			ImGui::OpenPopup("Delete?");
		}
		if (FileGroup::s_rename_item)
		{
			FileGroup::s_rename_item = false;
			ImGui::OpenPopup("Rename");
		}
	}
	DeletePopUp();
	RenamePopUp();
}

void FileGroup::ProjectViewPopUpOptions()
{
	bool item_selected = (FileGroup::s_hoveredPath != FileGroup::s_rootPath);
	if (ImGui::MenuItem("CreateFolder"))
	{
		std::filesystem::path p = std::filesystem::path((FileGroup::s_CurrentPath + "/newfolder").c_str());
		std::filesystem::create_directory(p);
		std::cout << (FileGroup::s_CurrentPath + "/newfolder") << std::endl;
	}
	if (ImGui::MenuItem("Delete", 0, false, item_selected))
	{
		FileGroup::s_delete_popup = true;
	}
#ifdef ENGINE_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Open File Location"))
	{
		std::filesystem::path p{ s_selectedpath };
		ShellExecuteA(NULL, "explore", p.parent_path().generic_u8string().c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#endif // ENGINE_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Copy"))
	{
		Editor::s_copyPayload.first = "FILE";
		Editor::s_copyPayload.second = Editor::s_bufferAllocator.New<char*>(FileGroup::s_selectedpath.data());
	}
	if (ImGui::MenuItem("Paste"))
	{
		if (Editor::s_copyPayload.first == "FILE")
		{
			//get the payload
			std::string temp = *static_cast<char**>(Editor::s_copyPayload.second);
			std::filesystem::path p(temp);
			Editor::s_bufferAllocator.Clear();

			std::filesystem::path selected_path{ FileGroup::s_selectedpath };
			std::string targetlocation_name;

			//changing the file name
			if (std::filesystem::is_directory(selected_path))
				targetlocation_name = FileGroup::s_selectedpath + "/copy" + p.filename().u8string();
			else
				targetlocation_name = selected_path.parent_path().u8string() + "/copy" + p.filename().u8string();

			if (std::filesystem::is_directory(p))
			{
				//copying the whole directory still requires work
				//std::filesystem::copy(p, targetlocation_name,
				//					  std::filesystem::copy_options::recursive |
				//					  std::filesystem::copy_options::overwrite_existing);
			}
			else
			{
				std::filesystem::copy(p, targetlocation_name);
			}
		}
	}
	if (ImGui::MenuItem("Rename"))
	{
		FileGroup::s_rename_item = true;
	}
}

void FileGroup::DeletePopUp()
{
	ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		std::filesystem::path p(FileGroup::s_selectedpath);
		ImGui::Text("Do you want to delete : \"%s\" ?", p.filename().generic_u8string().c_str());
		if (!std::filesystem::is_empty(p))
			ImGui::TextColored({ 1,0,0,1 }, "* The File Selected Is Not Empty *");
		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Yes", ImVec2{ 120,0 }))
		{
			ImGui::CloseCurrentPopup();
			FileGroup::s_CurrentPath = p.parent_path().generic_u8string();
			if (std::filesystem::is_directory(p))
				std::filesystem::remove_all(p);
			else
				std::filesystem::remove(p);
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2{ 120,0 }))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
void FileGroup::RenamePopUp()
{
	ImGui::SetNextWindowPos(FileGroup::s_selectedItemPosition);
	if (ImGui::BeginPopupModal("Rename", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::InputText("##RenameObject", FileGroup::s_nameBuffer, sizeof(FileGroup::s_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//rename item code here
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetKeyboardFocusHere();
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

