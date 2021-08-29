#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "ActionStack/EditorActionStack.h"
#include "ActionStack/InspectorActionBehaviour.h"

//engine code
#include "glm/gtc/type_ptr.hpp"
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/WorldManager.h"
#include "Engine/Asset/AssetsManager.h"
//components
#include "Engine/ECS/GameObjectComponent.h"
#include "Engine/Transform/Transform3D.h"
#include "Engine/Renderer/Sprite2D.h"
#include "Engine/PhysicsCollision/RigidBody.h"
#include "Engine/Prefab/EditorComponent.h"
#include "Engine/Prefab/PrefabComponent.h"
//libs
#include <imgui.h>
#include <imgui_internal.h>
#include <rttr/type>
#include <string>

using namespace rttr_type_ID;

InspectorView::InspectorView() :m_showReadOnly{ false },m_docked {false},m_addComponent{false}
{
}
void InspectorView::Show()
{
	ImGui::Begin("inspector",nullptr,ImGuiWindowFlags_MenuBar);
	ImGui::BeginMenuBar();
	if(ImGui::BeginMenu("Options"))
	{
		{//lock window UI
			auto* docknode = ImGui::FindWindowByName("inspector")->DockNode;
			if (docknode)
				m_docked = docknode->LocalFlags;
			if(ImGui::MenuItem("Lock Window",nullptr,m_docked, docknode))
			{
				if (docknode)
				{
					if (docknode->LocalFlags == 0)
						docknode->LocalFlags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
					else
						docknode->LocalFlags = 0;
				}
			}
		}
		if (ImGui::MenuItem("Show ReadOnly",nullptr,m_showReadOnly))
		{
			m_showReadOnly = !m_showReadOnly;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	ImGui::BeginChild("Preview items",ImVec2(0,ImGui::GetContentRegionMax().y - 75));
	{
		if (!ObjectGroup::s_FocusedObject)
		{
			ImGui::EndChild();
		}
		else
		{
			auto& go = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject);
			
			ShowGameObjectDetails(go);

			if(go.TryGetComponent<engine::Transform3D>())
				ReadComponents(go.GetComponent<engine::Transform3D>(),go);

			if (m_showReadOnly)
			{
				if (go.TryGetComponent<engine::EditorComponent>())
					ReadComponents(go.GetComponent<engine::EditorComponent>(), go);
			}
			AddComponentButton();
			ImGui::EndChild();
		}
	}

	ImGui::End();
}

void InspectorView::AddComponentButton()
{
	static const ImVec2 buttonSize = { 150,30 };
	static const ImVec2 DropDownListSize = { 300,200 };
	ImGui::NewLine();
	ImGui::SameLine((ImGui::GetContentRegionAvail().x - buttonSize.x) * 0.5f);
	if (ImGui::Button("Add Component",buttonSize))
	{
		m_addComponent = !m_addComponent;
	}
	if (m_addComponent)
	{
		ImGui::NewLine();
		ImGui::SameLine((ImGui::GetContentRegionAvail().x - DropDownListSize.x) * 0.5f);
		ComponentAddButton(DropDownListSize.x, DropDownListSize.y);

	}
}

void InspectorView::ShowGameObjectDetails(engine::GameObject& object)
{
	auto& goComponent = object.GetComponent<engine::GameObjectComponent>();
	auto& propName = goComponent.get_type().get_property("Name");
	auto& propActive = goComponent.get_type().get_property("Active");
	
	static char buf[100];
	strcpy(buf,object.Name().data());
	ImGui::Image((ImTextureID)engine::TextureDatabase::GetTexture("Ouroboros_Prefab").id, {50,50});
	ImGui::SameLine();

	ImGui::BeginGroup();
	if (ImGui::InputText("Name", buf, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoUndoRedo))
	{
		std::string temp = "Change value of element: " + propName.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name();
		rttr::variant undo = goComponent.Name.c_str();
		rttr::variant redo = buf;
		ActionStack::AllocateInBuffer(new InspectorActionBehaviour<engine::GameObjectComponent>{ temp, ObjectGroup::s_FocusedObject, propName, undo  , redo ,
									  object.GetComponent<engine::EditorComponent>().IsPrefabDirty() });
		object.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
		goComponent.Name = buf;
	}

	
	bool beforeActive = goComponent.ActiveSelf;
	if (ImGui::Checkbox("Active",&goComponent.ActiveSelf))
	{
		std::string temp = "Change value of element: " + propActive.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name();
		rttr::variant undo = beforeActive;
		rttr::variant redo = !beforeActive;
		ActionStack::AllocateInBuffer(new InspectorActionBehaviour<engine::GameObjectComponent>{ temp, ObjectGroup::s_FocusedObject, propActive, undo  , redo ,
									  object.GetComponent<engine::EditorComponent>().IsPrefabDirty() });
		object.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
		goComponent.ActiveSelf = !beforeActive;
	}
	ImGui::EndGroup();

	engine::EditorComponent& objectEC = object.GetComponent<engine::EditorComponent>();
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !objectEC.IsPrefab());
	objectEC.IsPrefab() ? ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f) : ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
	if (ImGui::Button("Update Prefab"))
	{
		objectEC.UpdatePrefab();
	}
	ImGui::SameLine();
	if (ImGui::Button("Break Prefab"))
	{
		objectEC.BreakOffFromPrefab();
	}
	ImGui::PopStyleVar();
	ImGui::PopItemFlag();
	ImGui::Separator();
}

void InspectorView::ComponentAddButton(float x ,float y)
{
	if (ImGui::BeginListBox("##Component", { x ,y }))
	{
		bool AddComponent = false;
		engine::GameObject go = ObjectGroup::s_FocusedObject;
		ImGui::BeginChild("##ListOfComponents", { x,y * 0.8f });
		{
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Transform3D>(go);
			AddComponent |= ComponentAddOptions<engine::Sprite2D>(go);
			AddComponent |= ComponentAddOptions<engine::RigidBody>(go);
		}
		ImGui::EndChild();

		{
			ImVec4 prevCol = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, {prevCol.x*0.2f,prevCol.y * 0.2f ,prevCol.z * 0.2f ,1});
			ImGui::BeginChild("##Description", {x,0});
			ImGui::TextWrapped("Transform Component");
			ImGui::EndChild();
			ImGui::PopStyleColor();
		}
		if (AddComponent)
			m_addComponent = false;
		ImGui::EndListBox();
	}
	
}

