#pragma once

#include <rttr/type.h>
#include <rttr/property.h>
#include <rttr/variant.h>
#include <vector>
#include "Engine/Transform/Transform3D.h"
#include "RttrTypeID.h"


class InspectorView
{
public:
	InspectorView();
	void Show();

private:

	template<typename Component>
	void ReadComponents(Component& component)
	{
		std::vector<rttr::property> types = component.get_type().get_properties();

		ImGui::BeginChild(component.get_type().get_name().c_str(), { 0,types.size() * 30.0f }, true);
		rttr::variant current_value;
		ImGui::PushID(ObjectGroup::s_FocusedObject);
		for (const rttr::property& element : types)
		{
			current_value.clear();
			const rttr::type::type_id id = element.get_type().get_id();
			if (id == m_tracked_ids[type_INT])
			{
				int value = element.get_value(component).get_value<int>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				if (ImGui::DragInt(element.get_name().c_str(), &value,  1, 0, 0, "%.3f", flag))
				{
					element.set_value(component, value);
				}
			}
			else if (id == m_tracked_ids[type_FLOAT])
			{
				float value = element.get_value(component).get_value<float>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				if (ImGui::DragFloat(element.get_name().c_str(), &value, 1, 0, 0, "%.3f", flag))
				{
					element.set_value(component, value);
				}
			}
			else if (id == m_tracked_ids[type_STRING])
			{
				static char buf[100];
				std::string value = element.get_value(component).get_value<std::string>();
				current_value = value;
				strcpy(buf, value.data());
				if (ImGui::InputText(element.get_name().c_str(), buf, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoUndoRedo))
				{
					value = buf;
					element.set_value(component, value);
					current_value = value;
				}
			}
			else if (id == m_tracked_ids[type_BOOL])
			{
				bool value = element.get_value(component).get_value<bool>();
				current_value = value;
				if (ImGui::RadioButton(element.get_name().c_str(), &value))
				{
					element.set_value(component, value);
				}
			}
			else if (id == m_tracked_ids[type_ENTITY])
			{
				ImGui::Text("%s : %u", element.get_name().c_str(), element.get_value(component).to_uint32());
			}
			else if (id == m_tracked_ids[type_VEC3])
			{
				glm::vec3 value = element.get_value(component).get_value<glm::vec3>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				if (ImGui::DragFloat3(element.get_name().c_str(), glm::value_ptr(value), 1, 0, 0, "%.3f", flag))
				{
					element.set_value(component, value);
				}
			}
			else if (id == m_tracked_ids[type_MAT4])
			{
				glm::mat4 value = element.get_value(component).get_value<glm::mat4>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				ImGui::Text(element.get_name().c_str());
				ImGui::PushID(element.get_name().c_str());
				bool activated = ImGui::DragFloat4("##x" , glm::value_ptr(value[0]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##y" , glm::value_ptr(value[1]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##z" , glm::value_ptr(value[2]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##w" , glm::value_ptr(value[3]),1,0,0,"%.3f", flag);
				ImGui::PopID();
				if (activated)
					element.set_value(component, value);
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
				if (ImGui::IsItemDeactivatedAfterEdit())
				{
					//redo stack
					redo.clear();
					redo = current_value;
					std::string temp = "Change value of element: " + element.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name;
					ActionStack::AllocateInBuffer(new InspectorActionBehaviour<Component>{ temp, ObjectGroup::s_FocusedObject, element, undo, redo });
				}
			}

		}
		ImGui::PopID();
		ImGui::EndChild();
	}
	
};

