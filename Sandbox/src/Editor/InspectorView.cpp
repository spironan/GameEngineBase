#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "ActionStack/EditorActionStack.h"
#include "ActionStack/InspectorActionBehaviour.h"
#include <imgui.h>
#include <rttr/type>

#include <string>
enum : int
{
	INT,
	FLOAT,
	STRING,
	BOOL
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
	if (!ObjectGroup::s_FocusedObject)
		return;

	ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });
	ImGui::Begin("inspector");
	{
		ImGui::Text("Name :  %s", ObjectGroup::s_FocusedObject->name.c_str());
		ReadComponents(ObjectGroup::s_FocusedObject->get_type());
	}
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
		if (id == m_tracked_ids[INT])
		{
			int value = element.get_value(ObjectGroup::s_FocusedObject).get_value<int>();
			current_value = value;
			if (ImGui::DragInt(element.get_name().c_str(), &value))
			{
				element.set_value(ObjectGroup::s_FocusedObject, value);

			}
		}
		else if (id == m_tracked_ids[FLOAT])
		{
			float value = element.get_value(ObjectGroup::s_FocusedObject).get_value<float>();
			current_value = value;
			if (ImGui::DragFloat(element.get_name().c_str(), &value))
			{
				element.set_value(ObjectGroup::s_FocusedObject, value);
			}
		}
		else if (id == m_tracked_ids[STRING])
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
				std::string temp = "Change value of element: " + element.get_name() + " of " + ObjectGroup::s_FocusedObject->name;
				ActionStack::AllocateInBuffer(new InspectorActionBehaviour{temp, ObjectGroup::s_FocusedObject, element, undo, redo });
			}
		}
		
	}
	ImGui::EndChild();
}
