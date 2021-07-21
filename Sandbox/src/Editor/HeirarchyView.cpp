/*****************************************************************//**
 * \file   HeirarchyView.cpp
 * \brief  Display the hierachy of the list of nodes
 *		   And allow interations with them.
 *		   
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#include "HeirarchyView.h"
#include "EditorObjectGroup.h"
#include "testclass.h"//remove once the real structure is in
#include "Editor.h"

#include <imgui.h>
#include <imgui_internal.h>

/**
 * \brief The main function to displaying the Hierarchy window
 *			-contains the function for the interaction of the nodes
 * 
 */
void HeirarchyView::Show()
{
	ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });//only works when undocked
	ImGui::Begin("Hierarchy");
	
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		ImGui::OpenPopup("HeirarchyViewPopUp");

	if (ImGui::BeginPopup("HeirarchyViewPopUp"))
	{
		HeirarchyPopUp();
		ImGui::EndPopup();
	}
	Search();
	ShowHeirarchy();

	ImGui::End();
}

void HeirarchyView::HeirarchyPopUp()
{
	if (ImGui::MenuItem("New Object"))
	{
		testclass tc{ 100 };
		tc.name = "new gameobject";
		Editor::s_testList.emplace_back(tc);
		Editor::s_testList.back().SetParent(&Editor::s_rootnode);
	}
	if (ImGui::MenuItem("Toggle lock UI"))
	{
		ToggleLockUI();
	}
}

void HeirarchyView::ShowHeirarchy()
{
	if (m_filtered == false)
	{
		for (testclass* obj : Editor::s_rootnode.childs)
			ListHeirarchy(obj);
	}
	else
	{
		for (testclass* obj : m_filterlist)
			ListHeirarchy(obj);
	}
}

void HeirarchyView::ListHeirarchy(testclass* obj)
{
	bool activated = false;
	ImGuiTreeNodeFlags flag = 0;

	
	if (ObjectGroup::s_FocusedObject == obj)
	{
		flag = ImGuiTreeNodeFlags_Selected;

		if (ImGui::IsKeyPressed(Editor::s_hotkeymapping[KEY_ACTIONS::RENAME_ITEM]))
			m_editing = true;

		if (m_dragging)
		{
			flag |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			m_dragging = !ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		}

		if (m_editing)
		{
			ImGui::PushID(obj->uid);
			if (ImGui::InputText("rename", m_Buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
			{
				obj->name = m_Buffer;
				m_Buffer[0] = '\0';//empty the character buffer
			}
			if (!ImGui::IsItemFocused())
				ImGui::SetKeyboardFocusHere();
			if (ImGui::IsItemDeactivated())//if clicked else where the textbox will dissapear
				m_editing = false;

			ImGui::PopID();
		}
	}
	flag |= (obj->childs.size()) ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	ImGui::PushID(obj->uid);
	activated = ImGui::TreeNodeEx((obj->name).c_str(), flag);
	ImGui::PopID();

	if (ImGui::IsItemClicked())
		ObjectGroup::s_FocusedObject = obj;
	//drop
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{
			m_dragging = false;
			ObjectGroup::s_FocusedObject->SetParent(obj);
		}
		ImGui::EndDragDropTarget();
	}
	//drag
	if (ObjectGroup::s_FocusedObject && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
	{
		// Set payload to carry the index of our item (could be anything)
		m_dragging = true;
		ImGui::SetDragDropPayload("HIERACHY_OBJ", nullptr, 0);
		ImGui::Text("%s", ObjectGroup::s_FocusedObject->name.c_str());
		ImGui::EndDragDropSource();
	}
	//creating childs(recurse)
	if (obj)//remove this when using the actual ver
	{

		if (activated && obj->childs.size() && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			for (testclass* item : obj->childs)
			{
				ListHeirarchy(item);
			}
			ImGui::TreePop();
		}
	}
}

void HeirarchyView::Search()
{
	if(ImGui::InputText("Search", m_filterBuffer, sizeof(m_filterBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (m_filterBuffer[0] == '\0')
		{
			m_filtered = false;
			m_filterlist.clear();
		}
		else
		{
			m_filtered = true;
			FilterByName(m_filterBuffer);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear"))
	{
		m_filterBuffer[0] = '\0';
		m_filtered = false;
		m_filterlist.clear();
	}
}

void HeirarchyView::FilterByName(const std::string& target)
{
	m_filterlist.clear();
	for (testclass& obj : Editor::s_testList)
	{
		if (obj.name.find(target) != std::string::npos)
			m_filterlist.emplace_back(&obj);//in the ecs case will be its id
	}
}

void HeirarchyView::ToggleLockUI()
{
	ImGuiWindow* window = ImGui::FindWindowByName("Hierarchy");
	if (window->DockNode->LocalFlags == 0)
		window->DockNode->LocalFlags = ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoTabBar;
	else
		window->DockNode->LocalFlags = 0;
}

