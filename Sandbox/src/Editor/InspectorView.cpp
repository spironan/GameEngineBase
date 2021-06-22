#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "EditorActionStack.h"

#include <imgui.h>
#include <rttr/type>

#include <iostream>
enum : int
{
	INT,
	FLOAT,
	CHAR,
	BOOL
};

InspectorView::InspectorView()
{
	m_tracked_ids.emplace_back(rttr::type::get<int>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<float>().get_id());
	m_tracked_ids.emplace_back(rttr::type::get<char>().get_id());
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
/**
 * \brief function used for changing the data (ElementData)
 * 
 * \param data
 *			ElementData only
 */
void InspectorView::SetElementData(void* data)
{
	ElementData* eD = static_cast<ElementData*>(data);
	ObjectGroup::s_FocusedObject = eD->item;
	eD->prop.set_value(eD->item, eD->data);
}

void InspectorView::ReadComponents(const rttr::type& _type)
{
	std::vector<rttr::property> types = _type.get_properties();

	ImGui::BeginChild(_type.get_name().c_str(), { 0,types.size() * 21.0f }, true);
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

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			//undo stack
			ElementData* data = ActionStack::AllocateInBuffer(ElementData{ ObjectGroup::s_FocusedObject, element, current_value });
			ActionStack::AddNewAction(SetElementData, data);
		}
		if(ImGui::IsItemFocused() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			//redo stack
			ElementData* data = ActionStack::AllocateInBuffer(ElementData{ ObjectGroup::s_FocusedObject, element, current_value });
			ActionStack::AppendRedoData(data);
		}
		
	}
	ImGui::EndChild();
}
