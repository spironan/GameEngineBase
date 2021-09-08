#include "InspectorView.h"
#include "../EditorObjectGroup.h"
#include "../Editor.h"

//action behaviour
#include "../ActionStack/EditorActionStack.h"
#include "../ActionStack/InspectorActionBehaviour.h"
#include "../ActionStack/ScriptActionStack.h"

////engine code
#include "Engine.h"
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
		auto& go = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject);
		if (!ObjectGroup::s_FocusedObject || !go.HasComponent<engine::GameObjectComponent>())
		{
			ImGui::EndChild();
		}
		else
		{
			ShowGameObjectDetails(go);

			if(auto* ptr = go.TryGetComponent<engine::Transform3D>())
				ReadComponents(*ptr,go);


			if (go.HasComponent<engine::SceneCamera>())
				ReadComponents(go.GetComponent<engine::SceneCamera>(), go);
			if (go.HasComponent<engine::Camera>())
				ReadComponents(go.GetComponent<engine::Camera>(), go);
			if (go.HasComponent<engine::Sprite2D>())
				ReadComponents(go.GetComponent<engine::Sprite2D>(), go);

			if (go.HasComponent<engine::Collider2D>())
				ReadComponents(go.GetComponent<engine::Collider2D>(), go);
			if (go.HasComponent<engine::CircleCollider2D>())
				ReadComponents(go.GetComponent<engine::CircleCollider2D>(), go);
			if (go.HasComponent<engine::BoxCollider2D>())
				ReadComponents(go.GetComponent<engine::BoxCollider2D>(), go);

			

			if (m_showReadOnly)
			{
				if (go.HasComponent<engine::EditorComponent>())
					ReadComponents(go.GetComponent<engine::EditorComponent>(), go);
			}

			ReadScriptInfo(go);
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
	ImGui::Image(engine::AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_Prefab")->Get_IMTEXTURE_ID(), {50,50});
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

	
	bool beforeActive = goComponent.Active;
	if (ImGui::Checkbox("Active",&goComponent.Active))
	{
		std::string temp = "Change value of element: " + propActive.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name();
		rttr::variant undo = beforeActive;
		rttr::variant redo = !beforeActive;
		ActionStack::AllocateInBuffer(new InspectorActionBehaviour<engine::GameObjectComponent>{ temp, ObjectGroup::s_FocusedObject, propActive, undo  , redo ,
									  object.GetComponent<engine::EditorComponent>().IsPrefabDirty() });
		object.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
		goComponent.Active = !beforeActive;
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
			AddComponent |= ComponentAddOptions<engine::Camera>(go);
			AddComponent |= ComponentAddOptions<engine::Sprite2D>(go);
			AddComponent |= ComponentAddOptions<engine::Rigidbody2D>(go);
			AddComponent |= ComponentAddOptions<engine::BoxCollider2D>(go);
			AddComponent |= ComponentAddOptions<engine::CircleCollider2D>(go);
			AddComponent |= ScriptAddOptions(go);
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

void InspectorView::ReadScriptInfo(engine::GameObject& object)
{
	if (object.TryGetComponent<engine::Scripting>() == false)
		return;
	auto& listScriptInfo = object.GetComponent<engine::Scripting>().GetScriptInfoAll();
	bool is_collapsed;

	engine::ScriptFieldInfo current_value;
	for (auto& info : listScriptInfo)
	{
		ImGui::BeginGroup();
		is_collapsed = (ImGui::TreeNodeEx(info.second.classInfo.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen) == false);
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50.0f);//button width
		if (ImGui::Button("Reset", ImVec2(0, ImGui::GetFontSize())))
		{
			//do smth
		}
		ImGui::EndGroup();
		if (is_collapsed)
			continue;
		ImGui::PushID(info.second.classInfo.ToString().c_str());
		for (auto& scriptVars : info.second.fieldMap)
		{
			engine::ScriptFieldInfo& fieldInfo = scriptVars.second;
			switch (fieldInfo.value.GetValueType())
			{
			case engine::ScriptValueType::EMPTY		  :
			{
				continue;
			}
			case engine::ScriptValueType::BOOL		  :
			{
				bool temp = fieldInfo.value.GetValue<bool>();
				current_value = fieldInfo;
				if(ImGui::RadioButton(fieldInfo.name.c_str(), temp))
				{
					fieldInfo.value.SetValue(!temp);
					current_value = fieldInfo;
				}
				break;
			}
			case engine::ScriptValueType::INT		  :
			{
				int temp = fieldInfo.value.GetValue<int>();
				current_value = fieldInfo;
				if (ImGui::DragInt(fieldInfo.name.c_str(), &temp))
				{
					fieldInfo.value.SetValue(temp);
					current_value = fieldInfo;
				}
				break;
			}
			case engine::ScriptValueType::FLOAT		  :
			{
				float temp = fieldInfo.value.GetValue<float>();
				current_value = fieldInfo;
				if (ImGui::DragFloat(fieldInfo.name.c_str(), &temp))
				{
					fieldInfo.value.SetValue(temp);
					current_value = fieldInfo;
				}
				break;
			}
			case engine::ScriptValueType::STRING	  :
			{
				std::string temp = fieldInfo.value.GetValue<std::string>();
				current_value = fieldInfo;

				static char buf[100];
					
				strcpy(buf, temp.data());
				if (ImGui::InputText(fieldInfo.name.c_str(), buf, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoUndoRedo))
				{
					fieldInfo.value.SetValue<std::string>(buf);
					current_value = fieldInfo;
				}
				break;
			}
			case engine::ScriptValueType::GAMEOBJECT  :
			{
				engine::Entity temp = fieldInfo.value.GetValue<engine::Entity>();
				ImGui::DragScalar(fieldInfo.name.c_str(), ImGuiDataType_::ImGuiDataType_U32, &temp, 1.0f, 0, 0, "%d", ImGuiSliderFlags_::ImGuiSliderFlags_NoInput);
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
					if (payload)
					{
						engine::Entity ent = *(reinterpret_cast<engine::Entity*>(payload->Data));
						fieldInfo.value.SetValue<engine::Entity>(ent);
						ImGui::EndDragDropTarget();
					}
				}
				break;
			}
			case engine::ScriptValueType::CLASS		  :
			{
				continue;
			}
			case engine::ScriptValueType::LIST		  :
			{
				continue;
			}
			}
			//undo and redo instructions
			{
				static engine::ScriptFieldInfo undo;
				static engine::ScriptFieldInfo redo;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					undo = current_value;
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
				{
					//redo stack
					redo = current_value;
					std::string temp = "Change value of element: " + undo.name + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name();

					ActionStack::AllocateInBuffer(new ScriptActionStack(temp,info.second.classInfo, ObjectGroup::s_FocusedObject ,undo,redo,
																		object.GetComponent<engine::EditorComponent>().IsPrefabDirty()));
					object.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
				}
			}
		}
		ImGui::PopID();
	}
}

bool InspectorView::ScriptAddOptions(engine::GameObject& go)
{
	const std::vector<engine::ScriptClassInfo>& listScriptInfo = engine::ScriptSystem::GetScriptClassList();

	for (auto& script : listScriptInfo)
	{
		bool selected = false;
		ImGui::BeginGroup();

		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f,0.9f,0,1.0f });
		ImGui::Selectable(script.ToString().c_str(), &selected);
		ImGui::PopStyleColor();

		ImGui::Separator();
		ImGui::EndGroup();
		if (selected)
		{
			go.EnsureComponent<engine::Scripting>();
			go.GetComponent<engine::Scripting>().AddScriptInfo(script);
			return true;
		}
	}
	return false;

}

