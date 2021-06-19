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
class testclass;
class HeirarchyView
{
public: 
	HeirarchyView() :m_dragging{ false }, m_editing{ false }, m_Buffer{""}{}
	void Show();
private:
	void HeirarchyPopUp();
	void ListHeirarchy(testclass* );
private:
	char m_Buffer[100];
	bool m_dragging;
	bool m_editing;
};

