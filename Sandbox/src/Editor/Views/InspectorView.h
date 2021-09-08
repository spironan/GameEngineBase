#pragma once

#include <rttr/type.h>
#include <rttr/property.h>
#include <rttr/variant.h>
#include <vector>
#include <imgui.h>
#include "Engine/Transform/Transform3D.h"
#include "Engine/Prefab/EditorComponent.h"
#include "../RttrTypeID.h"

#include <iostream>
class InspectorView
{
public:
	InspectorView();
	void Show();

private:
	
	void AddComponentButton();
	void ShowGameObjectDetails(engine::GameObject& object);

	void ComponentAddButton(float x ,float y);

	void ReadScriptInfo(engine::GameObject&);

	template<typename Component>
	void ReadComponents(Component& component,engine::GameObject& object)
	{
		bool is_readonly;
		bool is_open;

		std::vector<rttr::property> types = component.get_type().get_properties();
		rttr::variant current_value;

		ImGui::BeginGroup();
		is_open = ImGui::TreeNodeEx(component.get_type().get_name().c_str(), ImGuiTreeNodeFlags_DefaultOpen |ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50.0f);//button width
		if (object.HasComponent<Component>())
		{
			ImGui::PushID(component.get_type().get_name().c_str());
			if (ImGui::Button("Remove", ImVec2(0,ImGui::GetFontSize())))
			{
				object.RemoveComponent<Component>();
			}
			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::Separator();
		if (!is_open)
		{
			return;
		}
		ImGui::Dummy({ 5,0 });//indent spacing
		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::PushID(component.get_type().get_name().c_str());
		for (const rttr::property& element : types)
		{
			is_readonly = element.is_readonly();
			if (is_readonly)
			{
				if (m_showReadOnly)
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.5f,0.5f,0.5f,1 });
				else
					continue;
			}
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
				bool s_value = element.get_value(component).get_value<bool>();
				current_value = s_value;
				if (ImGui::RadioButton(element.get_name().c_str(), s_value))
				{
					element.set_value(component, !s_value);
				}
			}
			else if (id == m_tracked_ids[type_ENTITY])
			{
				ImGui::Text("%s : %u", element.get_name().c_str(), element.get_value(component).to_uint32());
			}
			else if (id == m_tracked_ids[type_VEC3])
			{
				oom::vec3 value = element.get_value(component).get_value<oom::vec3>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				if (ImGui::DragFloat3(element.get_name().c_str(), oom::value_ptr(value), 1, 0, 0, "%.3f", flag))
				{
					element.set_value(component, value);
				}
			}
			else if (id == m_tracked_ids[type_MAT4])
			{
				oom::mat4 value = element.get_value(component).get_value<oom::mat4>();
				ImGuiSliderFlags flag = 0;
				if (element.is_readonly())
					flag = ImGuiSliderFlags_ReadOnly | ImGuiSliderFlags_NoInput;
				else
					current_value = value;
				ImGui::Text(element.get_name().c_str());
				ImGui::PushID(element.get_name().c_str());
				bool activated = ImGui::DragFloat4("##x" , oom::value_ptr(value[0]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##y" , oom::value_ptr(value[1]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##z" , oom::value_ptr(value[2]),1,0,0,"%.3f", flag);
				activated = ImGui::DragFloat4("##w" , oom::value_ptr(value[3]),1,0,0,"%.3f", flag);
				ImGui::PopID();
				if (activated)
					element.set_value(component, value);
			}
			//new types add here



			//these should be the last 
			else if (element.is_enumeration())
			{
				rttr::variant value = element.get_value(component);
				
				if (!element.is_readonly())
					current_value = value;

				auto& listNames = element.get_enumeration().get_names();
				auto& listValues = element.get_enumeration().get_values();
				
				char temp[64];
				std::strcpy(temp, element.get_enumeration().value_to_name(value).data());
				ImGui::PushID(element.get_name().c_str());
				ImGui::InputText("##Identifyer", temp, 64, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();

				static bool dropdown = false;
				if (ImGui::Button("Expand"))
					dropdown = !dropdown;
				if (dropdown)
				{
					if (ImGui::BeginListBox(element.get_name().c_str()))
					{
						bool selected ;
						for (size_t iter = 0; iter < listNames.size(); ++iter)
						{
							selected = (listValues[iter] == value);
							if (ImGui::Selectable(listNames[iter].c_str(), &selected))
							{
								element.set_value(component,listValues[iter]);
								current_value = value;
							}
							if(selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}
				}
				ImGui::PopID();
			}
			else if (element.get_type().get_properties().size())
			{
				ImGui::Dummy({ 5,0 });//give some padding
				ImGui::SameLine();
				ImGui::BeginGroup();
				ReadComponents(element.get_value(component), object);
				ImGui::EndGroup();
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
					std::string temp = "Change value of element: " + element.get_name() + " of " + static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name();
					
					ActionStack::AllocateInBuffer(new InspectorActionBehaviour<Component>{ temp, ObjectGroup::s_FocusedObject, element, undo, redo,
												  object.GetComponent<engine::EditorComponent>().IsPrefabDirty() });
					object.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
				}
			}
			if (is_readonly)
				ImGui::PopStyleColor();

		}
		ImGui::EndGroup();
		ImGui::PopID();
	}
	
	template<typename Component>
	bool ComponentAddOptions(engine::GameObject& go)
	{
		rttr::type t = rttr::type::get<Component>();
		bool selected = false;
		ImGui::BeginGroup();
		ImGui::Selectable(t.get_name().c_str(), &selected);
		ImGui::Separator();
		ImGui::EndGroup();
		if (selected)
		{
			go.AddComponent<Component>();
			return true;
		}
		return false;
	}

	bool ScriptAddOptions(engine::GameObject& go);

private:
	bool m_showReadOnly; //editor settings
	bool m_docked;
	bool m_addComponent;
};

