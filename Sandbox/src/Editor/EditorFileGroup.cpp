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
#include "Engine/Core/KeyCode.h"

#include "EditorFileGroup.h"
#include "Editor.h"

#include <imgui_internal.h>
#include <filesystem>

#include <windows.h>

const std::string FileGroup::s_rootPath = "./";
std::string FileGroup::s_CurrentPath = s_rootPath;
std::string FileGroup::s_hoveredPath = s_rootPath;
std::string FileGroup::s_selectedpath;//path with the itemname
std::string FileGroup::s_selecteditem;//itemname only
ImVec2 FileGroup::s_targetItemPosition;//vec2
char FileGroup::s_nameBuffer[128] = "";//for renaming items

const ImGuiID FileGroup::s_projectviewid = 100;
const ImGuiID FileGroup::s_renamefolderid = 101;

bool FileGroup::s_delete_popup = false;//flag for deleting modal popup

bool FileGroup::s_rename_item = false;

//archived
//std::vector<std::string> FileGroup::s_currentfolderview;

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
	}
	DeletePopUp();
	RenamePopUp();
}
/**
 * \brief 
 *	the file is used for widgets which handles filesystem
 *	 - shortcut used are ctrl + c and ctrl + v
 */
void FileGroup::KeyshortCuts()
{
	if (ImGui::IsWindowHovered())
	{
		if (ImGui::GetIO().KeyCtrl)
		{
			if (ImGui::IsKeyPressed(static_cast<int>(engine::KeyCode::C)))
				FileGroup::CopyItem();
			else if (ImGui::IsKeyPressed(static_cast<int>(engine::KeyCode::V)))
				FileGroup::PasteItem();
		}
		if (ImGui::IsKeyPressed(static_cast<int>(engine::KeyCode::F2)) && FileGroup::s_selectedpath != FileGroup::s_rootPath)
		{
			s_targetItemPosition = ImGui::GetMousePos();
			ImGui::OpenPopupEx(s_renamefolderid);
		}
	}
}

///**
//* \brief
//* after setting current path call this function to preload all the filenames
//* so as to reduce the need to reiterate all the items
//* this function is required to be call everytime there are changes made to the file
//*/
//void FileGroup::LoadFolderItems()
//{
//	s_currentfolderview.clear();
//	for (auto& entry : std::filesystem::directory_iterator(FileGroup::s_CurrentPath))
//	{
//		s_currentfolderview.emplace_back(entry.path().filename().u8string());
//	}
//}
void FileGroup::ProjectViewPopUpOptions()
{
	bool item_selected = (FileGroup::s_hoveredPath != FileGroup::s_rootPath);
	if (ImGui::MenuItem("CreateFolder"))
	{
		std::string fileTemp = (FileGroup::s_CurrentPath + "/Newfolder");
		std::string current = fileTemp;
		int count = 0;
		while (std::filesystem::exists(current))
		{
			++count;
			current = fileTemp + " (" + std::to_string(count) + ")";
		}
		std::filesystem::create_directory(current);
	}
	if (ImGui::MenuItem("Delete", 0, false, item_selected))
	{
		FileGroup::s_delete_popup = true;
	}
#ifdef SANDBOX_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Open File Location"))
	{
		std::filesystem::path p{ s_selectedpath };
		ShellExecuteA(NULL, "explore", p.parent_path().generic_u8string().c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#endif // SANDBOX_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Copy"))
	{
		FileGroup::CopyItem();
	}
	if (ImGui::MenuItem("Paste"))
	{
		FileGroup::PasteItem();
	}
	if (ImGui::MenuItem("Rename"))
	{
		ImGui::OpenPopupEx(s_renamefolderid);
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
	ImGui::SetNextWindowPos(FileGroup::s_targetItemPosition);
	
	if (ImGui::BeginPopupEx(s_renamefolderid, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::InputText("##RenameObject", FileGroup::s_nameBuffer, sizeof(FileGroup::s_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//rename item code here
			std::filesystem::path p{ FileGroup::s_hoveredPath };
			std::string changeName = (p.parent_path().u8string() + "/" + FileGroup::s_nameBuffer + p.extension().u8string());

			if (std::filesystem::exists(changeName) == false)//if is unique name then rename file else dont accept
			{
				std::filesystem::rename(p, changeName);
				FileGroup::s_nameBuffer[0] = '\0';//clear the buffer

				//if the path is invalid change it to the new path
				if (!std::filesystem::exists(FileGroup::s_selectedpath))
					s_selectedpath = changeName;
				if (!std::filesystem::exists(FileGroup::s_CurrentPath))
					s_CurrentPath = changeName;
				FileGroup::s_hoveredPath = changeName;
				ImGui::CloseCurrentPopup();
			}
			else
			{
				WarningView::DisplayWarning(WarningView::DisplayType::DISPLAY_ERROR, "Invalid Name");
#ifdef SANDBOX_PLATFORM_WINDOWS
				MessageBeep(0x00000030L);//Windows Exclamation sound.
#endif // SANDBOX_PLATFORM_WINDOWS
			}
		}
		ImGui::SetKeyboardFocusHere();
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			FileGroup::s_nameBuffer[0] = '\0';//clear the buffer
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
void FileGroup::CopyItem()
{
	Editor::s_payloadBufferAllocator.Clear();//clear before use
	Editor::s_copyPayload.first = "FILE";
	//+1 due to '\0'
	Editor::s_copyPayload.second = Editor::s_payloadBufferAllocator.NewArr<char>(FileGroup::s_selectedpath.size() + 1, engine::BufferAllocator::ALIGNMENT);
	sprintf_s(static_cast<char*>(Editor::s_copyPayload.second), FileGroup::s_selectedpath.size() + 1, FileGroup::s_selectedpath.data());
	
}
void FileGroup::PasteItem()
{
	if (Editor::s_copyPayload.first == "FILE")
	{
		//get the payload
		std::filesystem::path p((static_cast<char*>(Editor::s_copyPayload.second)));

		std::filesystem::path selected_path{ FileGroup::s_selectedpath };
		std::string targetlocation_name;

		//changing the file name

		targetlocation_name = selected_path.parent_path().u8string() + "/" + p.stem().u8string() + " - copy";

		//iteratively look for a new file name(might get expensive when there is more than a certain amount of files)
		std::string newfile_name = targetlocation_name + p.extension().u8string();
		int counter = 0;
		while (std::filesystem::exists(newfile_name))
		{
			++counter;
			newfile_name = targetlocation_name + " (" + std::to_string(counter) + ") " + p.extension().u8string();
		}

		if (std::filesystem::is_directory(p) && !std::filesystem::is_empty(p))
		{
			//copying the whole directory still requires work
			//std::filesystem::copy(p, targetlocation_name,
			//					  std::filesystem::copy_options::recursive |
			//					  std::filesystem::copy_options::overwrite_existing);
		}
		else
		{
			std::filesystem::copy(p, newfile_name);
		}
	}
}
