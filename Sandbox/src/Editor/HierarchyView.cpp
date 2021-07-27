/*****************************************************************//**
 * \file   HierarchyView.cpp
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
#include "HierarchyView.h"
#include "EditorObjectGroup.h"
#include "Editor.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/WorldManager.h"
#include <imgui.h>
#include <imgui_internal.h>

/**
 * \brief The main function to displaying the Hierarchy window
 *			-contains the function for the interaction of the nodes
 * 
 */
void HierarchyView::Show()
{
	ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });//only works when undocked
	ImGui::Begin("Hierarchy");
	
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		ImGui::OpenPopup("HierarchyViewPopUp");

	if (ImGui::BeginPopup("HierarchyViewPopUp"))
	{
		HierarchyPopUp();
		ImGui::EndPopup();
	}
	Search();
	ShowHierarchy();

	ImGui::End();
}

void HierarchyView::HierarchyPopUp()
{
	if (ImGui::MenuItem("New Object"))
	{
		engine::GameObject go;
		go.Name = "New GameObject";
	}
	if (ImGui::MenuItem("Toggle lock UI"))
	{
		ToggleLockUI();
	}
}

void HierarchyView::ShowHierarchy()
{
	if (m_filtered == false)
	{
		ListHierarchy();
	}
	else
	{
		bool activated = false;
		ImGuiTreeNodeFlags flag;
		std::vector<std::uint32_t> depth;
		depth.emplace_back(0);//root
		int treePop = 0;// cleanly pops the tree at the end if there is any poping required

		for (size_t i = 0; i < m_filterlist.size(); ++i)
		{
			engine::Transform3D& transform = static_cast<engine::GameObject>(m_filterlist[i]).GetComponent<engine::Transform3D>();
			if (transform.GetEntity() == 0) continue;

			flag = 0;
			if (ObjectGroup::s_FocusedObject == transform.GetEntity())
			{
				flag = ImGuiTreeNodeFlags_Selected;
				if (m_dragging)
				{
					flag |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
					m_dragging = !ImGui::IsMouseReleased(ImGuiMouseButton_Left);
				}
			}
			//check if theres an error here TODO
			while (depth.back() != transform.GetParentId())
			{
				//if (depth.size() == 1)
				//	return;
				std::uint32_t temp = depth.back();
				depth.pop_back();
				ImGui::TreePop();
				--treePop;
			}
			if (transform.GetChildCount())
			{
				flag |= ImGuiTreeNodeFlags_OpenOnArrow;

				ImGui::PushID(transform.GetEntity());
				activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name.c_str(), flag);
				ImGui::PopID();
				//if activated then show child else skip
				(activated && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen)) ? ++treePop, depth.emplace_back(transform.GetEntity()) : i += transform.GetChildCount();

			}
			else
			{
				flag |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				ImGui::PushID(transform.GetEntity());
				activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name.c_str(), flag);
				ImGui::PopID();
			}

			if (ImGui::IsItemClicked())
				ObjectGroup::s_FocusedObject = transform.GetEntity();
		}
		//clear up the remaining branching 
		while (treePop)
		{
			--treePop;
			ImGui::TreePop();
		}
	}
}

void HierarchyView::ListHierarchy()
{
	bool activated = false;
	ImGuiTreeNodeFlags flag = 0;
	int treePop = 0;
	std::vector<std::uint32_t> depth;
	depth.emplace_back(0);//root
	auto& transformList = engine::WorldManager::GetActiveWorld().GetComponentDenseArray<engine::Transform3D>();
	for (size_t i = 0; i < transformList.size() ; ++i)
	{
		engine::Transform3D& transform = transformList[i];
		if (transform.GetEntity() == 0) continue;

		flag = 0;
		if (ObjectGroup::s_FocusedObject == transform.GetEntity())
		{
			flag = ImGuiTreeNodeFlags_Selected;
			if (m_dragging)
			{
				flag |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
				m_dragging = !ImGui::IsMouseReleased(ImGuiMouseButton_Left);
			}
		}
		//check if theres an error here TODO
		while (depth.back() != transform.GetParentId())
		{
			//if (depth.size() == 1)
			//	return;
			std::uint32_t temp = depth.back();
			depth.pop_back();
			ImGui::TreePop();
			--treePop;
		}
		if (transform.GetChildCount())
		{
			flag |= ImGuiTreeNodeFlags_OpenOnArrow;
			
			ImGui::PushID(transform.GetEntity());
			activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name.c_str(), flag);
			ImGui::PopID();
			//if activated then show child else skip
			(activated && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen)) ? ++treePop , depth.emplace_back(transform.GetEntity()) : i += transform.GetChildCount();

		}
		else
		{
			flag |=	 ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::PushID(transform.GetEntity());
			activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name.c_str(), flag);
			ImGui::PopID();
		}

		if (ImGui::IsItemClicked())
			ObjectGroup::s_FocusedObject = transform.GetEntity();
		//drop
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
			if (payload)
			{
				m_dragging = false;
				static_cast<engine::GameObject>(transform.GetEntity()).AddChild(ObjectGroup::s_FocusedObject);
				ImGui::EndDragDropTarget();
				break;
			}
			ImGui::EndDragDropTarget();
		}
		//drag
		if (ObjectGroup::s_FocusedObject && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
		{
			// Set payload to carry the index of our item (could be anything)
			m_dragging = true;
			ImGui::SetDragDropPayload("HIERACHY_OBJ", nullptr, 0);
			ImGui::Text("%s", static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name.c_str());
			ImGui::EndDragDropSource();
		}
	}
	//clear up the remaining branching 
	while (treePop)
	{
		--treePop;
		ImGui::TreePop();
	}
}

void HierarchyView::Search()
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

void HierarchyView::FilterByName(const std::string& target)
{
	m_filterlist.clear();
	auto& transformList = engine::WorldManager::GetActiveWorld().GetComponentDenseArray<engine::Transform3D>();
	for (engine::Transform3D& transform : transformList)
	{
		engine::GameObject& ent = static_cast<engine::GameObject>(transform.GetEntity());
		if (ent.Name.find(target) != std::string::npos)
		{
			m_filterlist.emplace_back(ent);
		}
	}
}

void HierarchyView::ToggleLockUI()
{
	ImGuiWindow* window = ImGui::FindWindowByName("Hierarchy");
	if (window->DockNode->LocalFlags == 0)
		window->DockNode->LocalFlags = ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoTabBar;
	else
		window->DockNode->LocalFlags = 0;
}

