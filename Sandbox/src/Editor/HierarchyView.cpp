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
#include "EditorFileGroup.h"
#include "Editor.h"
#include "Engine/Scene/SceneManager.h"

#include "Seralizer.h"

//action stack stuff
#include "ActionStack/EditorActionStack.h"//add and remove action
#include "ActionStack/ParentActionStack.h"

#include <imgui.h>
#include <imgui_internal.h>

/**
 * \brief The main function to displaying the Hierarchy window
 *			-contains the function for the interaction of the nodes
 * 
 */
void HierarchyView::Show()
{
	//ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });//only works when undocked
	ImGui::Begin("Hierarchy");

	Search();
	ShowHierarchy();

	ImGui::End();
}

void HierarchyView::HierarchyPopUp()
{
	if (ImGui::MenuItem("New Object"))
	{
		engine::GameObject ent;
		(ent).AddComponent<engine::Transform3D>();
		engine::GameObject(engine::SceneManager::GetActiveRoot()).AddChild(ent);
	}
	if (ImGui::MenuItem("Copy"))
	{
		Copy(ObjectGroup::s_FocusedObject);
	}
	if (ImGui::MenuItem("Paste",nullptr,nullptr, m_CopyTarget))
	{
		Paste();
	}
	if (ImGui::MenuItem("Delete",nullptr,nullptr,ObjectGroup::s_FocusedObject))
	{
		static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Destroy();
		ObjectGroup::s_FocusedObject = 0;
	}
	ImGui::Separator();
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
		depth.emplace_back(engine::SceneManager::GetActiveRoot());//root
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
			while (!depth.empty() && depth.back() != transform.GetParentId())
			{
				std::uint32_t temp = depth.back();
				depth.pop_back();
				ImGui::TreePop();
				--treePop;
			}
			if (transform.GetChildCount())
			{
				flag |= ImGuiTreeNodeFlags_OpenOnArrow;

				ImGui::PushID(transform.GetEntity());
				activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name().c_str(), flag);
				ImGui::PopID();
				//if activated then show child else skip
				(activated && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen)) ? ++treePop, depth.emplace_back(transform.GetEntity()) : i += transform.GetChildCount();

			}
			else
			{
				flag |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				ImGui::PushID(transform.GetEntity());
				activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name().c_str(), flag);
				ImGui::PopID();
			}

			if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
				ObjectGroup::s_FocusedObject = transform.GetEntity();
		}
		//clear up the remaining branching 
		while (treePop)
		{
			--treePop;
			ImGui::TreePop();
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
			ImGui::OpenPopup("HierarchyViewPopUp");
		if (ImGui::BeginPopup("HierarchyViewPopUp"))
		{
			HierarchyPopUp();
			ImGui::EndPopup();
		}
	}
}

void HierarchyView::ListHierarchy()
{
	bool activated = false;
	bool showTree = false;
	//default flag for the first node
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen ;
	int treePop = 0;
	std::vector<std::uint32_t> depth;
	engine::Entity root = engine::SceneManager::GetActiveRoot();//todo use scenemanager to get rootnode after its ready
	//depth.emplace_back(root);//root
	auto& transformList = engine::SceneManager::GetActiveScene().GetWorld().GetComponentDenseArray<engine::Transform3D>();
	ImGui::BeginChild("##ListHierarchy");
	//display the root node
	if (transformList.size())
	{
		ImGui::Separator();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f,0.5f,0.5f,1.0f });
		showTree = ImGui::TreeNodeEx(engine::GameObject(root).Name().c_str(), flag);
		SetParent(root);

		ImGui::PopStyleColor();
		ImGui::Separator();

		//if tree is not open nothing is shown
		if (!showTree)
			return;
		depth.emplace_back(root);
		++treePop;
	}

	for (auto iter = transformList.begin() + 1; iter != transformList.end(); ++iter)//skip the root node
	{
		flag = 0;//reset flag before use
		engine::Transform3D& transform = *iter;
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
		while (!depth.empty() && depth.back() != transform.GetParentId())
		{
			depth.pop_back();
			ImGui::TreePop();
			--treePop;
		}
		if (transform.GetChildCount())
		{
			flag |= ImGuiTreeNodeFlags_OpenOnArrow;
			ImGui::PushID(transform.GetEntity());
			activated = ImGui::TreeNodeEx(engine::GameObject(transform.GetEntity()).Name().c_str(), flag);
			ImGui::PopID();
			//if activated then show child else skip
			(activated && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen)) ? ++treePop , depth.emplace_back(transform.GetEntity()) : std::advance(iter,transform.GetChildCount());

		}
		else
		{
			flag |=	 ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			engine::Entity tempid = transform.GetEntity();
			ImGui::PushID(tempid);
			activated = ImGui::TreeNodeEx(engine::GameObject(tempid).Name().c_str(), flag);
			ImGui::PopID();
		}
		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ObjectGroup::s_FocusedObject = transform.GetEntity();

		//drop
		if (SetParent(transform.GetEntity()))
			break;
		//drag
		if (ObjectGroup::s_FocusedObject && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
		{
			// Set payload to carry the index of our item (could be anything)
			m_dragging = true;
			ImGui::SetDragDropPayload("HIERACHY_OBJ", nullptr, 0);
			ImGui::Text("%s", static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).Name().c_str());
			ImGui::EndDragDropSource();
		}
	}
	//clear up the remaining branching 
	while (treePop)
	{
		--treePop;
		ImGui::TreePop();
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		ImGui::OpenPopup("HierarchyViewPopUp");
	if (ImGui::BeginPopup("HierarchyViewPopUp"))
	{
		HierarchyPopUp();
		ImGui::EndPopup();
	}

	ImGui::EndChild();

	SetParent(root);
	KeyCopy(ObjectGroup::s_FocusedObject);
	KeyPaste();
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
	auto& transformList = engine::SceneManager::GetActiveScene().GetWorld().GetComponentDenseArray<engine::Transform3D>();
	for (engine::Transform3D& transform : transformList)
	{
		engine::GameObject& ent = static_cast<engine::GameObject>(transform.GetEntity());
		if (ent.Name().find(target) != std::string::npos)
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

bool HierarchyView::SetParent(engine::Entity entt)
{
	if (ImGui::BeginDragDropTarget())
	{
		//hierarchy object payload
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{
			m_dragging = false;
			engine::Entity parent_id = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).GetComponent<engine::Transform3D>().GetParentId();
			ActionStack::AllocateInBuffer(new ParentActionStack(ObjectGroup::s_FocusedObject, parent_id, entt));
			static_cast<engine::GameObject>(entt).AddChild(ObjectGroup::s_FocusedObject);
			ImGui::EndDragDropTarget();
			return true;
		}
		//prefab payload
		payload = ImGui::AcceptDragDropPayload("PREFAB_OBJ");
		if (payload)
		{
			std::string temp;
			temp = reinterpret_cast<char*>(payload->Data);
			Serializer::LoadObject(temp.c_str(),entt);
			ImGui::EndDragDropTarget();
			return true;
		}
		ImGui::EndDragDropTarget();
	}
	return false;
}

void HierarchyView::KeyCopy(engine::Entity ent)
{
	if (ImGui::IsKeyDown(static_cast<int>(engine::Key::LCTRL)) && ImGui::IsKeyPressed(static_cast<int>(engine::Key::C)))
	{
		Copy(ent);
	}
}

void HierarchyView::KeyPaste()
{
	if (ImGui::IsKeyDown(static_cast<int>(engine::Key::LCTRL)) && ImGui::IsKeyPressed(static_cast<int>(engine::Key::V)))
	{
		Paste();
	}
}

void HierarchyView::Copy(engine::Entity entt)
{
	m_CopyTarget = entt;
}

void HierarchyView::Paste()
{
	if (m_CopyTarget == 0)
		return;
	engine::Transform3D targetTransform = static_cast<engine::GameObject>(m_CopyTarget).GetComponent<engine::Transform3D>();

	int childcount = targetTransform.GetChildCount();
	//create the parent node first
	engine::GameObject parent;
	parent.Name() += "-Copy";

	//update this once the function is done TODO
	auto& trans = parent.GetComponent<engine::Transform3D>();
	trans.SetPosition(targetTransform.GetPosition());
	trans.SetScale(targetTransform.GetScale());
	trans.SetRotationAngle(targetTransform.GetRotationAngle());
	trans.SetRotationAxis(targetTransform.GetRotationAxis());
	/** *********************************************************** */
	if (childcount == 0)
		return;

	auto& tranformList = engine::SceneManager::GetActiveWorld().GetComponentDenseArray<engine::Transform3D>();
	//change this later on once this the Function to get all childs are done TODO
	size_t iter = 0;
	while (++iter != tranformList.size())//skip the first node since its the root node
	{
		if (tranformList[iter].GetEntity() == m_CopyTarget)
			break;
	}
	/** **************************************************************************/
	engine::Entity prevParent = parent;
	std::vector<std::pair<engine::Entity, engine::Entity>> hierarchy;
	engine::Entity entID = engine::SceneManager::GetActiveRoot();
	for (iter += 1; iter < tranformList.size(); ++iter)//increase iter by 1 to skip the parent node
	{
		engine::GameObject child;
		auto& trans = child.GetComponent<engine::Transform3D>();
		auto& newTrans = tranformList[iter];
		{
			trans.SetPosition(newTrans.GetPosition());
			trans.SetScale(newTrans.GetScale());
			trans.SetRotationAngle(newTrans.GetRotationAngle());
			trans.SetRotationAxis(newTrans.GetRotationAxis());
		}
		{
			const engine::Entity parentid = newTrans.GetParentId();
			while (true)
			{
				if (hierarchy.empty())
				{
					prevParent = parent;
					break;
				}
				else
				{
					if (parentid != hierarchy.back().second)
						hierarchy.pop_back();
					else
					{
						prevParent = hierarchy.back().first;
						break;
					}
				}
			}
		}
		static_cast<engine::GameObject>(prevParent).AddChild(child);
		if (newTrans.GetChildCount())
		{
			hierarchy.emplace_back(child.GetID(), newTrans.GetID());
			prevParent = child;
		}
		--childcount;
		if (childcount == 0)
			break;
	}
	engine::SceneManager::GetActiveRoot().AddChild(parent);
}


