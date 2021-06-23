#include "InspectorView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "EditorActionStack.h"

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
/**
 * \brief 
 *		function for cleaning the data assigned
 *		some data structures might be too complex
 *		that just doing a delete on a void* will not work
 * 
 * \param data
 *		the function is to clean up the data
 */
void InspectorView::DeleteElementData(void* data)
{
	ElementData* eD = static_cast<ElementData*>(data);
	eD->data.clear();//there might be occasions where the variant only stores a pointer to it
	delete eD;
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
			std::string value = element.get_value(ObjectGroup::s_FocusedObject).get_value<std::string>();
			current_value = value;
			
			if (ImGui::InputText(element.get_name().c_str(),value.data(),100,ImGuiInputTextFlags_EnterReturnsTrue| ImGuiInputTextFlags_NoUndoRedo))
			{
				element.set_value(ObjectGroup::s_FocusedObject, value);
				current_value = value;
			}
		}
		//undo and redo instructions
		{
			static ElementData undo = ElementData(nullptr, element, 0);
			static ElementData redo = ElementData(nullptr, element, 0);
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				//undo stack
				undo.item = ObjectGroup::s_FocusedObject;
				undo.prop = element;
				undo.data.clear();
				undo.data = current_value;
			}
			if(ImGui::IsItemDeactivatedAfterEdit())
			{
				//redo stack
				std::cout << "deactivated after edit\n";
				redo.item = ObjectGroup::s_FocusedObject;
				redo.prop = element;
				redo.data.clear();
				redo.data = current_value;
				ActionStack::AddNewAction(SetElementData,undo,redo,DeleteElementData);
			}
		}
		
	}
	ImGui::EndChild();
}
