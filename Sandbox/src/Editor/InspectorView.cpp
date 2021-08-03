#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "ActionStack/EditorActionStack.h"
#include "ActionStack/InspectorActionBehaviour.h"

//engine code
#include "glm/gtc/type_ptr.hpp"
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObjectComponent.h"
#include "Engine/Transform/Transform3D.h"
//libs
#include <imgui.h>
#include <imgui_internal.h>
#include <rttr/type>
#include <string>

using namespace rttr_type_ID;

InspectorView::InspectorView()
{
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
			
			ImGui::Text("Name :  %s", go.Name().c_str());
			if(go.TryGetComponent<engine::GameObjectComponent>())
				ReadComponents(go.GetComponent<engine::GameObjectComponent>());
			if(go.TryGetComponent<engine::Transform3D>())
				ReadComponents(go.GetComponent<engine::Transform3D>());
			ImGui::Button("Add Component");
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


