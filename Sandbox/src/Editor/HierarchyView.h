/*****************************************************************//**
 * \file   HierarchyView.h
 * \brief  Manages and updates the object in the game scene
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "Engine/Transform/Transform3D.h"
class HierarchyView
{
public: 
	HierarchyView() : m_dragging{ false },
		m_editing{ false },
		m_filtered{ false },
		m_rename{ false },
		m_Buffer{ "" }, m_filterBuffer{ "" }
	{
	};
	void Show();

private:
	void HierarchyPopUp();

	void ShowHierarchy();
	void ListHierarchy();
	void ListFiltered();
	void Search();
	void FilterByName(const std::string& target);
	void ToggleLockUI();
	bool SetParent(engine::Entity entt);

	void ShortCutKeys();
	void Copy();
	void Paste();
	void Rename();
private:


	bool m_dragging;
	bool m_editing;
	bool m_filtered;
	bool m_rename;

	char m_Buffer[100];
	char m_filterBuffer[100];


	engine::Entity m_CopyTarget = 0;//for ctrl + c 
	std::vector<engine::Entity> m_filterlist;

};

