/*****************************************************************//**
 * \file   ProjectFolderView.cpp
 * \brief  Display and contains all the interations in ProjectFolderView
 *		   window.
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
 *********************************************************************/
//sandbox
#include "ProjectFolderView.h"
#include "Editor/Editor.h"
#include "EditorFileGroup.h"
#include "EditorObjectGroup.h"
//engine
#include "Engine/Scene/SceneManager.h"
#include "Engine/Prefab/PrefabComponentSystem.h"

//util
#include "RttrTypeID.h"
#include "Seralizer.h"
#include "Engine/Asset/AssetsManager.h"
#include "Utility/Hash.h"

//imgui
#include <imgui.h>

//std
#include <string>
#include <filesystem>

//windows
#include <Windows.h>
#include <shellapi.h>
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
	//when scrolled
	float scroll_count = io.MouseWheel;
	if (io.KeyCtrl && (scroll_count))
	{
		scroll_count = abs(scroll_count) > 1 ? scroll_count * 0.5f : scroll_count / abs(scroll_count);
		size_multiplier -= static_cast<int>(scroll_count);
		size_multiplier = size_multiplier < 1 ? 1 : size_multiplier;//min
		size_multiplier = size_multiplier > 4 ? 4 : size_multiplier;//max
		padding = max_padding / size_multiplier;//determin the padding when scrolled
		imgsize = max_imgsize / size_multiplier;
	}
	//table calculation
	float row = ImGui::GetContentRegionAvailWidth() / (padding + imgsize);
	if (row < 1)//when changing tabs this will be set to false
		return;
	
	//show directory (recursive function)
	ImGui::BeginChild("preview_directory", { ImGui::GetContentRegionAvail().x ,35 }, true);
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

		/// <summary> Icons for file thumbnail
		/// help generate icons corresponding the the file ext
		/// </summary>
		IconButtons(entry.path().extension().u8string(),imgsize);

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
					InteractionWithFile(entry.path());
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
			std::string filename = FileGroup::s_CurrentPath+ "/" + engine::GameObject(ObjectGroup::s_FocusedObject).Name() + ".prefab";
			engine::SceneManager::GetActiveWorld().GetSystem<engine::PrefabComponentSystem>()->MakePrefab(filename, engine::GameObject(ObjectGroup::s_FocusedObject));
		}
		ImGui::EndDragDropTarget();
	}

}



void ProjectFolderView::PathDir(std::filesystem::path& entry, std::string& path)
{

	static engine::utility::StringHash::size_type strhash = engine::utility::StringHash(FileGroup::s_rootPath);
	static char count = 0;
	static float fontWidth = ImGui::CalcTextSize("a").x;
	bool selected = false;
	if (count == 0)
	{
		if(ImGui::Button("Home"))
			path = FileGroup::s_rootPath;
		ImGui::SameLine();
	}

	if (engine::utility::StringHash(entry.u8string().c_str()) != strhash && 
		entry.has_parent_path())
	{
		++count;
		if (count < 5)
			PathDir(entry.parent_path(), path);
		std::string temp = entry.stem().u8string();
		ImGui::Selectable(temp.c_str(), &selected, 0, { fontWidth * temp.size(),0 });
		ImGui::SameLine();
		ImGui::Bullet();
		--count;
		if(selected)
			path = entry.u8string();
	}


	
}

void ProjectFolderView::SearchFilter()
{
	static char buffer[100];

	ImGui::BeginChild("Search", { ImGui::GetContentRegionAvail().x * 0.4f,30});
	if (ImGui::InputText("Search", buffer, sizeof(buffer),ImGuiInputTextFlags_AutoSelectAll))
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
			temp = path.u8string() + '\0';
			ImGui::SetDragDropPayload("PREFAB_OBJ", temp.c_str(), temp.size());
			ImGui::Text("%s", temp.c_str());
		}
		ImGui::EndDragDropSource();
	}
}

void ProjectFolderView::InteractionWithFile(const std::filesystem::path& path)
{
	using HashType = engine::utility::StringHash::size_type;
	static HashType extHash[] = {	engine::utility::StringHash(".prefab"),
									engine::utility::StringHash(".ini")
								};
	HashType ht = engine::utility::StringHash(path.extension().u8string().c_str());
	if (ht == extHash[0])
	{

	}
	else if (ht == extHash[1])
	{

	}
	else
	{
		std::string temp = path.u8string();
		ShellExecuteA(NULL, "open" ,temp.c_str(), NULL, NULL, SW_SHOW);
	}
}


bool ProjectFolderView::IconButtons(const std::string& ext , float imgsize)
{
	static std::vector<engine::utility::StringHash::size_type> ext_hash{
		engine::utility::StringHash(".png"),
		engine::utility::StringHash(".wav"),
		engine::utility::StringHash(".mp3"),
		engine::utility::StringHash(".prefab")
	};
	bool selected;
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
	if (ext.empty())
	{
		auto tex = engine::AssetManager::GetAsset<engine::Texture>("Icons/Ouroboros_Folder.png");
		selected = ImGui::ImageButton(tex->Get_IMTEXTURE_ID(), {imgsize, imgsize});
		ImGui::PopStyleColor();
		return selected;
	}
	
	engine::utility::StringHash::size_type curr_item = engine::utility::StringHash(ext);

	if(curr_item == ext_hash[0])
		selected = ImGui::ImageButton(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_PNG")->Get_IMTEXTURE_ID(), { imgsize, imgsize });
	else if(curr_item == ext_hash[1])
		selected = ImGui::ImageButton(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_WAV")->Get_IMTEXTURE_ID(), { imgsize, imgsize });
	else if (curr_item == ext_hash[2])
		selected = ImGui::ImageButton(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_MP3")->Get_IMTEXTURE_ID(), { imgsize, imgsize });
	else if (curr_item == ext_hash[3])
		selected = ImGui::ImageButton(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_Prefab")->Get_IMTEXTURE_ID(), { imgsize, imgsize });
	else
		selected = ImGui::ImageButton(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_GenericFile")->Get_IMTEXTURE_ID(), { imgsize, imgsize });

	ImGui::PopStyleColor();
	return selected;
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
