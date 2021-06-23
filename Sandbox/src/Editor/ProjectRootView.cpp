/*****************************************************************//**
 * \file   ProjectRootView.cpp
 * \brief  Display the selected folders from project root view
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#include "ProjectRootView.h"
#include "EditorFileGroup.h"
#include "Editor.h"
#include <imgui.h>
#include <rttr/type>
#include <filesystem>

#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <shellapi.h>

/**
 * \brief
 *		display the project root view
 *		project root view shows only folders(directories)
 *	this function contains code for:
 *		showing the root view window
 *		triggering the popup when right clicking
 */
void ProjectRootView::Show()
{
	ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });
	ImGui::Begin("Project Dir");
	ProjectView(FileGroup::s_rootPath, FileGroup::s_CurrentPath);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		if (ImGui::IsAnyItemHovered() == false)
			FileGroup::s_hoveredPath = FileGroup::s_rootPath;
		ImGui::OpenPopup(FileGroup::s_projectviewid);
	}
	ImGui::End();
}





void ProjectRootView::ProjectView(const std::string& path, std::string& selected_dir)
{

	ImGuiTreeNodeFlags flag = 0;

	//this is to get a img to draw
	ImGuiIO& io = ImGui::GetIO();
	ImFontAtlas* atlas = io.Fonts;

	char counter = 0;
	bool enable = false;
	for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory())//if item is a folder use the folder img
		{
			continue;
		}
		ImGui::Image(atlas->TexID, { 10,10 });
		flag = ImGuiTreeNodeFlags_OpenOnArrow;

		ImGui::SameLine();

		if (layer == curr)
		{
			if (entry.path().filename().generic_u8string() == FileGroup::s_selecteditem)
			{
				flag |= ImGuiTreeNodeFlags_Selected;
			}

		}

		enable = ImGui::TreeNodeEx(entry.path().filename().generic_u8string().c_str(), flag);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) || (ImGui::IsItemClicked(ImGuiMouseButton_Left)))
		{
			if (entry.is_directory())//for the previde module
				selected_dir = entry.path().generic_u8string();

			//to mark the item that is focused
			FileGroup::s_selecteditem = entry.path().filename().generic_u8string();
			FileGroup::s_selectedpath = entry.path().generic_u8string();
			FileGroup::s_selectedItemPosition = ImGui::GetMousePos();

			layer = curr;
		}
		if (ImGui::IsItemHovered())
		{
			FileGroup::s_hoveredPath = entry.path().u8string();
		}

		if (enable && entry.is_directory())
		{
			ImGui::PushID(counter);
			curr.emplace_back(counter);
			ProjectView(entry.path().u8string(), selected_dir);
			curr.pop_back();
			ImGui::PopID();
			ImGui::TreePop();//pop the tree nodes
		}
		++counter;
	}
}


