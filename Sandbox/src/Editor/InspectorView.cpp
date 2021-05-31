#include "InspectorView.h"
#include "Editor.h"
#include <imgui.h>
#include <rttr/type>
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
	if (!Editor::s_FocusedObject)
		return;

	ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });
	ImGui::Begin("inspector");
	{
		ImGui::Text("Name :  %s", Editor::s_FocusedObject->name.c_str());
		//for(auto comp:components)
		{
			ReadComponents(Editor::s_FocusedObject->get_type());
		}
	}
	ImGui::End();
}

void InspectorView::ReadComponents(const rttr::type& _type)
{

	std::vector<rttr::property> types = _type.get_properties();

	ImGui::BeginChild(_type.get_name().c_str(), { 200,200 }, true);
	for (const rttr::property& element : types)
	{
		const rttr::type::type_id& id = element.get_type().get_id();
		if (id == m_tracked_ids[INT])
		{
			int value = element.get_value(Editor::s_FocusedObject).get_value<int>();
			ImGui::SliderInt(element.get_name().c_str(), &value, 0, 10);
			element.set_value(Editor::s_FocusedObject, value);
		}
		else if (id == m_tracked_ids[FLOAT])
		{
			float value = element.get_value(Editor::s_FocusedObject).get_value<int>();
			ImGui::SliderFloat(element.get_name().c_str(), &value, 0, 10);
			element.set_value(Editor::s_FocusedObject, value);
		}
		
	}
	ImGui::EndChild();
}
