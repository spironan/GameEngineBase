#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "ActionStack/EditorActionStack.h"
#include "ActionStack/InspectorActionBehaviour.h"

//engine code
#include "Engine/Transform/Transform3D.h"
#include "Engine/ECS/GameObject.h"

//libs
#include <imgui.h>
#include <imgui_internal.h>
#include <rttr/type>
#include <string>
enum : int
{
	enum_INT,
	enum_FLOAT,
	enum_STRING,
	enum_BOOL
};

InspectorView::InspectorView()
{
	m_tracked_ids.emplace_back(rttr::type::get<int>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<float>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<std::string>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<bool>().get_id());
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
			ImGui::Text("Name :  %s", static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name);
			//ReadComponents(ObjectGroup::s_FocusedObject->get_type());
			ImGui::EndChild();
		}
	}

	//ImGui::BeginChild("Ending bar");
	//static bool docked = ImGui::FindWindowByName("inspector")->DockNode->LocalFlags;
	//ImGui::Separator();
	//if (ImGui::RadioButton("Toggle Lock",docked))
	//{
	//	auto& docknode = ImGui::FindWindowByName("inspector")->DockNode;
	//	if (docknode->LocalFlags == 0)
	//		docknode->LocalFlags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
	//	else
	//		docknode->LocalFlags = 0;
	//}
	//ImGui::EndChild();
	ImGui::End();
}

void InspectorView::ReadComponents(const rttr::type& _type)
{
	std::vector<rttr::property> types = _type.get_properties();

	ImGui::BeginChild(_type.get_name().c_str(), { 0,types.size() * 30.0f }, true);
	rttr::variant current_value;

	for (const rttr::property& element : types)
	{
		current_value.clear();
		const rttr::type::type_id id = element.get_type().get_id();
		if (id == m_tracked_ids[enum_INT])
		{
			int value = element.get_value(ObjectGroup::s_FocusedObject).get_value<int>();
			current_value = value;
			if (ImGui::DragInt(element.get_name().c_str(), &value))
			{
				element.set_value(ObjectGroup::s_FocusedObject, value);

			}
		}
		else if (id == m_tracked_ids[enum_FLOAT])
		{
			float value = element.get_value(ObjectGroup::s_FocusedObject).get_value<float>();
			current_value = value;
			if (ImGui::DragFloat(element.get_name().c_str(), &value))
			{
				element.set_value(ObjectGroup::s_FocusedObject, value);
			}
		}
		else if (id == m_tracked_ids[enum_STRING])
		{
			static char buf[100];
			std::string value = element.get_value(ObjectGroup::s_FocusedObject).get_value<std::string>();
			current_value = value;
			strcpy(buf, value.data());
			if (ImGui::InputText(element.get_name().c_str(),buf,100,ImGuiInputTextFlags_EnterReturnsTrue| ImGuiInputTextFlags_NoUndoRedo))
			{
				value = buf;
				element.set_value(ObjectGroup::s_FocusedObject, value);
				current_value = value;
			}
		}
		//undo and redo instructions
		{
			static rttr::variant undo;
			static rttr::variant redo;
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				undo.clear();
				undo = current_value;
			}
			if(ImGui::IsItemDeactivatedAfterEdit())
			{
				//redo stack
				redo.clear();
				redo = current_value;
				std::string temp = "Change value of element: " + element.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name;
				ActionStack::AllocateInBuffer(new InspectorActionBehaviour{temp, ObjectGroup::s_FocusedObject, element, undo, redo });
			}
		}
		
	}
	ImGui::EndChild();
}
