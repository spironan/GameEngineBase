/*****************************************************************//**
 * \file   ProjectFolderView.cpp
 * \brief  Display and contains all the interations in ProjectFolderView
 *		   window.
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
 *********************************************************************/
#include "ProjectFolderView.h"
#include "Editor/Editor.h"
#include "EditorFileGroup.h"
#include "EditorObjectGroup.h"

#include "RttrTypeID.h"
#include "Seralizer.h"

#include <imgui.h>
#include <string>
#include <filesystem>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

/**
 * \brief
 *		main function to display the Project Folder window.
 * 
 */
void ProjectFolderView::Show()
{

	ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });

	ImGui::Begin("Project Folder");
	ProjectView();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		if (ImGui::IsAnyItemHovered() == false)
			FileGroup::s_hoveredPath = FileGroup::s_rootPath;
		ImGui::OpenPopup(FileGroup::s_projectviewid);
	}
	FileGroup::KeyshortCuts();
	ImGui::End();

}



void ProjectFolderView::ProjectView()
{
	//remove this once integrated to the rendering system
	ImGuiIO& io = ImGui::GetIO(); // use for scrolling
	ImFontAtlas* atlas = io.Fonts;

	//padding
	static const float max_padding = 20;
	static const float max_imgsize = 128;

	static float padding = max_padding;
	static float imgsize = max_imgsize;

	static int size_multiplier = 1;

	//local variables
	std::filesystem::directory_iterator dir_iter = std::filesystem::directory_iterator(FileGroup::s_CurrentPath);
	bool selected = false;
	//when scrolled
	float scroll_count = io.MouseWheel;
	if (io.KeyCtrl && (scroll_count))
	{
		scroll_count = abs(scroll_count) > 1 ? scroll_count * 0.5f : scroll_count / abs(scroll_count);
		size_multiplier -= static_cast<int>(scroll_count);
		size_multiplier = size_multiplier < 1 ? 1 : size_multiplier;//min
		size_multiplier = size_multiplier > 7 ? 7 : size_multiplier;//max
		padding = max_padding / size_multiplier;//determin the padding when scrolled
		imgsize = max_imgsize / size_multiplier;
	}
	//table calculation
	float row = ImGui::GetContentRegionAvailWidth() / (padding + imgsize);
	if (row < 1)//when changing tabs this will be set to false
		return;
	
	//show directory (recursive function)
	ImGui::BeginChild("preview_directory", { ImGui::GetContentRegionAvail().x ,30 }, true);
	PathDir(std::filesystem::path(FileGroup::s_CurrentPath), FileGroup::s_CurrentPath);
	ImGui::EndChild();
	//search bar
	SearchFilter();

	//if begin table fails return
	if (ImGui::BeginTable("preview_table", static_cast<int>(row)) == false)
		return;

	ImGui::TableNextColumn();//push 1 column first
	std::string fileNameTemp;
	for (auto& entry : dir_iter)
	{
		//filter search
		if (m_filtering && entry.path().filename().u8string().find(m_filter) == std::string::npos)
			continue;

		ImGui::PushID(entry.path().u8string().c_str());
		ImGui::BeginGroup();//start
		//change this would be changed once rendering is integrated
		if (entry.path().filename().has_extension() == false)
			selected = ImGui::ImageButton(atlas->TexID, { imgsize, imgsize }, { 0,0 }, { 0.125,1 });
		else
			selected = ImGui::ImageButton(atlas->TexID, { imgsize, imgsize });
		/// <summary> Drag and drop
		/// this scope will contain all the drag from file to (xxx) payload creation
		/// </summary>
		FileBeginDrag(entry.path());

		//text of the filename
		fileNameTemp = entry.path().filename().generic_u8string();
		//do not shorten name when item is hovered
		if(!ImGui::IsItemHovered())
			TextProcessing(fileNameTemp, imgsize*2.0f);

		ImGui::TextWrapped(fileNameTemp.c_str());
		ImGui::EndGroup();//end
		ImGui::PopID();

		ImGui::TableNextColumn();//item done

		//interactions of item
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				if (entry.is_directory())
				{
					FileGroup::s_CurrentPath = entry.path().generic_u8string().c_str();
					ImGui::EndTable();
					return;
				}
				//can open c# file here
				else if (entry.path().has_extension())
				{
					std::string a = entry.path().generic_u8string().c_str();
					//TODO remove this in place of a better code
					//std::system(a.substr(2).c_str());//substr can be removed one we have a proper filepath
				}
			}
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				FileGroup::s_targetItemPosition = ImGui::GetMousePos();
				FileGroup::s_selectedpath = entry.path().u8string();
				FileGroup::s_selecteditem = entry.path().filename().u8string();
			}
			FileGroup::s_hoveredPath = entry.path().u8string();
		}
	}
	ImGui::EndTable();
	//drag and drop interaction for prefab
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{
			Serializer::SaveObject(engine::GameObject(ObjectGroup::s_FocusedObject).Name+".prefab");
		}
		ImGui::EndDragDropTarget();
	}

}



void ProjectFolderView::PathDir(std::filesystem::path& entry, std::string& path)
{
	static int count = 0;
	bool selected = false;

	if (entry.has_parent_path() && count < 5)
	{
		++count;
		PathDir(entry.parent_path(), path);
		//it is currently hardcoded
		ImGui::Selectable(entry.filename().generic_u8string().c_str(), &selected, 0, { 7.0f * entry.filename().generic_u8string().size(),13 });
		ImGui::SameLine();
		ImGui::Bullet();
	}
	if (count)//exception for the last object
	{
		--count;
		ImGui::SameLine();
	}
	if (selected)
		path = entry.generic_u8string().c_str();
}

void ProjectFolderView::SearchFilter()
{
	static char buffer[100];

	ImGui::BeginChild("Search", { ImGui::GetContentRegionAvail().x * 0.4f,30});
	if (ImGui::InputText("##Filter", buffer, sizeof(buffer),ImGuiInputTextFlags_AutoSelectAll))
	{
		m_filtering = true;
		m_filter = buffer;
	}
	if (ImGui::IsItemActivated())
		WarningView::DisplayWarning(WarningView::DISPLAY_LOG, "**Note** : Case-Sensitive Search");
	if (buffer[0] == '\0')//if the user empty the buffer
		m_filtering = false;
	ImGui::EndChild();

}
/*********************************************************************************//*!
\brief    This Fuction will require constant updating as there will be more file
		  interation support
 
\param    path

*//**********************************************************************************/
void ProjectFolderView::FileBeginDrag(const std::filesystem::path& path)
{
	if (ImGui::IsItemActive() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
	{
		std::string temp = path.extension().u8string();
		if (temp == ".prefab")
		{
			temp = path.u8string();
			ImGui::SetDragDropPayload("PREFAB_OBJ", temp.c_str(), temp.size());
			ImGui::Text("%s", temp.c_str());
		}
		ImGui::EndDragDropSource();
	}
}

void ProjectFolderView::TextProcessing(std::string& str, float windowSize)
{
	size_t characters =static_cast<size_t>(floor(windowSize / ImGui::GetFontSize()));
	if (str.size() < characters)
		return;
	if (characters <= 4)
	{
		str.resize(2);
		str += ".\0";
	}
	else
	{
		str.resize(characters - 3);
		str += "..\0";
	}
}
