#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "ActionStack/EditorActionStack.h"
#include "ActionStack/InspectorActionBehaviour.h"

//engine code
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Transform/Transform3D.h"
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/WorldManager.h"
//libs
#include <imgui.h>
#include <imgui_internal.h>
#include <rttr/type>
#include <string>


InspectorView::InspectorView()
{
	m_tracked_ids.emplace_back(rttr::type::get<int>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<float>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<std::string>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<bool>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<engine::Entity>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<glm::vec3>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<glm::mat4>().get_id());
}
void InspectorView::Show()
{
	ImGui::Begin("inspector");
	ImGui::BeginChild("Preview items",ImVec2(0,ImGui::GetContentRegionMax().y - 75));
	{
		if (!ObjectGroup::s_FocusedObject)
		{
			ImGui::EndChild();
		}
		else
		{
			auto& go = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject);
			
			ImGui::Text("Name :  %s", go.Name.c_str());
			
			ReadComponents(go.GetComponent<engine::Transform3D>());//hard coded to read only 1 component
			ImGui::EndChild();
		}
	}

	ImGui::BeginChild("Ending bar");
	auto* dock = ImGui::FindWindowByName("inspector")->DockNode;
	if(dock != nullptr)
		static bool docked = dock->LocalFlags;
	ImGui::Separator();
	if (ImGui::RadioButton("Toggle Lock", dock))
	{
		auto* docknode = ImGui::FindWindowByName("inspector")->DockNode;
		if (docknode != nullptr) 
		{
			if (docknode->LocalFlags == 0)
				docknode->LocalFlags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
			else
				docknode->LocalFlags = 0;
		}
	}
	ImGui::EndChild();
	ImGui::End();
}


