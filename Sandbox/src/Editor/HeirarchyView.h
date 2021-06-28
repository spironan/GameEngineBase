/*****************************************************************//**
 * \file   HeirarchyView.h
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
class testclass;
class HeirarchyView
{
public: 
	HeirarchyView() :m_dragging{ false }, m_editing{ false }, m_filtered{ false }, m_Buffer{ "" }, m_filterBuffer{ "" }{}
	void Show();
private:
	void HeirarchyPopUp();

	void ShowHeirarchy();
	void ListHeirarchy(testclass* );
	void Search();
	void FilterByName(const std::string& target);
private:
	bool m_dragging;
	bool m_editing;
	bool m_filtered;

	char m_Buffer[100];
	char m_filterBuffer[100];
	std::vector<testclass*> m_filterlist;
};

