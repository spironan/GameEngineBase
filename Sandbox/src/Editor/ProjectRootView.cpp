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

void ProjectRootView::Show()
{
	ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });
	ImGui::Begin("Project Dir");
	ProjectView(FileGroup::s_rootPath, FileGroup::s_CurrentPath);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		if (ImGui::IsAnyItemHovered() == false)
			FileGroup::s_hoveredPath = FileGroup::s_rootPath;
		ImGui::OpenPopup("ProjectViewPopup");
	}

	if (ImGui::BeginPopup("ProjectViewPopup"))
	{
		ProjectViewPopUp();
		ImGui::EndPopup();
		if (delete_popup)
		{
			delete_popup = false;
			ImGui::OpenPopup("Delete?");
		}
	}
	ProjectDeletePopUp_Modal();
	ImGui::End();


}

void ProjectRootView::ProjectViewPopUp()
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
		delete_popup = true;
	}
#ifdef ENGINE_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Open File Location"))
	{
		std::filesystem::path p{ m_selectedpath };
		ShellExecuteA(NULL, "explore", p.parent_path().generic_u8string().c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#endif // ENGINE_PLATFORM_WINDOWS
	ImGui::Separator();
	if (ImGui::MenuItem("Copy"))
	{
		Editor::s_copyPayload.first = "FILE";
		Editor::s_copyPayload.second.reset();
		Editor::s_copyPayload.second = std::make_shared<void*>(&m_selectedpath);
	}
	if (ImGui::MenuItem("Paste"))
	{
		if (Editor::s_copyPayload.first == "FILE")
		{
			std::filesystem::path p(*(static_cast<std::string*>(*Editor::s_copyPayload.second.get())));
			Editor::s_copyPayload.second.reset();

			std::filesystem::path selected_path{ m_selectedpath };
			std::string targetlocation_name;

			if (std::filesystem::is_directory(selected_path))
				targetlocation_name = m_selectedpath + "/copy" + p.filename().u8string();
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
		m_rename_item = true;
	}
}

void ProjectRootView::ProjectDeletePopUp_Modal()
{	
	ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		std::filesystem::path p(m_selectedpath);
		ImGui::Text("Do you want to delete : \"%s\" ?", p.filename().generic_u8string().c_str());
		if (!std::filesystem::is_empty(p))
			ImGui::TextColored({ 1,0,0,1 }, "* The File Selected Is Not Empty *");
		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Yes", ImVec2{120,0}))
		{
			ImGui::CloseCurrentPopup();
			FileGroup::s_CurrentPath = p.parent_path().generic_u8string();
			if (std::filesystem::is_directory(p))
				std::filesystem::remove_all(p);
			else
				std::filesystem::remove(p);
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2{120,0}))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

}

void ProjectRootView::RenameCallback()
{
	if (m_rename_item)
	{
		ImGui::SetKeyboardFocusHere();
		if (ImGui::InputText("rename", m_nameBuffer, sizeof(m_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//rename file
			m_rename_item = false;
		}
	}
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
		if (entry.is_directory())//if item is a folder use the folder img
		{
			ImGui::Image(atlas->TexID, { 10,10 });
			flag = ImGuiTreeNodeFlags_OpenOnArrow;
		}
		else//use some img
		{
			ImGui::Image(atlas->TexID, { 20,10 });
			flag = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}
		ImGui::SameLine();

		if (layer == curr)
		{
			if (entry.path().filename().generic_u8string() == m_selecteditem)
			{
				flag |= ImGuiTreeNodeFlags_Selected;
				for (size_t iter = 0; iter < m_runOnSelected.size(); ++iter)
				{
					(this->*m_runOnSelected[iter])();
				}
					
			}

		}

		enable = ImGui::TreeNodeEx(entry.path().filename().generic_u8string().c_str(), flag);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) || (ImGui::IsItemClicked(ImGuiMouseButton_Left)))
		{
			if (entry.is_directory())//for the previde module
				selected_dir = entry.path().generic_u8string();

			//to mark the item that is focused
			m_selecteditem = entry.path().filename().generic_u8string();
			m_selectedpath = entry.path().generic_u8string();
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


