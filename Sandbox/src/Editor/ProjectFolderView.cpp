#include "ProjectFolderView.h"
#include "Editor/Editor.h"
#include "EditorFileGroup.h"
#include "EditorObjectGroup.h"

#include <imgui.h>
#include <string>
#include <filesystem>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>


void ProjectFolderView::Show()
{

	ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });

	ImGui::Begin("Project Folder");
	ProjectView();
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
		size_multiplier = size_multiplier > 7 ? 7 : size_multiplier;//max
		padding = max_padding / size_multiplier;//determin the padding when scrolled
		imgsize = max_imgsize / size_multiplier;
	}
	
	//show directory
	ImGui::BeginChild("preview_directory", { ImGui::GetContentRegionAvailWidth(),30 }, true);
	PathDir(std::filesystem::path(FileGroup::s_CurrentPath), FileGroup::s_CurrentPath);
	ImGui::EndChild();

	//table calculation
	float row = ImGui::GetContentRegionAvailWidth() / (padding + imgsize);
	if (row < 1|| ImGui::BeginTable("preview_table", static_cast<int>(row)) == false)//when changing tabs this will be set to false
		return;
	ImGui::TableNextColumn();//push 1 column first
	for (std::filesystem::directory_entry entry : dir_iter)
	{
		ImGui::BeginGroup();//start
		//change this would be changed once rendering is integrated
		if (entry.path().filename().has_extension() == false)
			ImGui::ImageButton(atlas->TexID, { imgsize, imgsize }, { 0,0 }, { 0.125,1 });
		else
			ImGui::ImageButton(atlas->TexID, { imgsize, imgsize });
		//text of the filename
		ImGui::TextWrapped(entry.path().filename().generic_u8string().c_str());
		ImGui::EndGroup();//end
		ImGui::TableNextColumn();//item done

		//interactions of item
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
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
				std::system(a.substr(2).c_str());//substr can be removed one we have a proper filepath
			}
		}
	}
	ImGui::EndTable();
	//drag and drop interaction for prefab
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{
			std::ofstream stream("prefab");
			rapidjson::OStreamWrapper osw(stream);
			rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
			writer.StartObject();
			SaveHeirarchy(ObjectGroup::s_FocusedObject, writer);
			writer.EndObject();
		}
		ImGui::EndDragDropTarget();
	}
}

void ProjectFolderView::SaveHeirarchy(testclass* object, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	SaveObject(object, writer);
	for (int i = 0; i < object->childs.size(); ++i)
	{
		SaveHeirarchy(object->childs[i], writer);
	}
}

void ProjectFolderView::SaveObject(testclass* object, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();

	writer.Key(std::to_string(object->uid).c_str());
	writer.StartArray();

	writer.Key(object->get_type().get_name().c_str());
	writer.StartArray();//start component 1
	for (int j = 0; j < list.size(); ++j)
	{
		if (list[j].get_type() == rttr::type::get<int>())
		{
			writer.Int(list[j].get_value(*object).to_int());
		}
		else if (list[j].get_type() == rttr::type::get<std::string>())
		{
			writer.String(list[j].get_value(*object).to_string().c_str());
		}
	}
	writer.EndArray();//end component 1
	writer.EndArray();
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
