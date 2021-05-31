#include "ProjectRootView.h"
#include "Editor.h"
#include <imgui.h>
#include <rttr/type>
#include <filesystem>
void ProjectRootView::Show()
{
	ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });
	ImGui::Begin("Project Dir");
	ProjectView("./", Editor::s_CurrentPath);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		ImGui::OpenPopup("ProjectViewPopup");

	if (ImGui::BeginPopup("ProjectViewPopup"))
	{
		ProjectViewPopUp();
		ImGui::EndPopup();
	}
	ImGui::End();
}

void ProjectRootView::ProjectViewPopUp()
{
	if (ImGui::MenuItem("CreateFolder"))
	{
		std::filesystem::path p = std::filesystem::path((Editor::s_CurrentPath + "/newfolder").c_str());
		std::filesystem::create_directory(p);
		std::cout << (Editor::s_CurrentPath + "/newfolder") << std::endl;
	}
	if (ImGui::MenuItem("Delete", 0, false, (Editor::s_CurrentPath != "./")))
	{
		//learn how to delete files properly
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
				flag |= ImGuiTreeNodeFlags_Selected;
		}

		enable = ImGui::TreeNodeEx(entry.path().filename().generic_u8string().c_str(), flag);

		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
		{
			if (entry.is_directory())//for the previde module
				selected_dir = entry.path().generic_u8string();
			//to mark the item that is focused
			m_selecteditem = entry.path().filename().generic_u8string();
			layer = curr;
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


