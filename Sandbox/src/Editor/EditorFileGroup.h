/*****************************************************************//**
 * \file   EditorFileGroup.h
 * \brief  
 *		   Contains functions and variables shared among ProjectRootView
 *		   and ProjectFolderView
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
#include <imgui.h>
class FileGroup
{
public:
	FileGroup() {};
	~FileGroup() {};

	const static std::string s_rootPath;
	static std::string s_CurrentPath;
	static std::string s_hoveredPath;
	static std::string s_selectedpath;//path with the itemname
	static std::string s_selecteditem;//itemname only
	static ImVec2 s_targetItemPosition;//vec2
	static char s_nameBuffer[128];//for renaming items


	static const ImGuiID s_projectviewid;
	static const ImGuiID s_renamefolderid;
	static bool s_delete_popup;//flag for deleting modal popup

	static bool s_rename_item;
public:
	/**
	* \brief 
	* this code will be activated when right clicking inside the
	* folder view or rootview windows
	* \note  
	* This function should not be called by anyone other than
	* the main display function
	*/
	static void ProjectViewPopUp();
	/**
	* \brief
	* the file is used for widgets which handles filesystem
	* shortcut used are ctrl + c and ctrl + v
	*/
	static void KeyshortCuts();

	/*********************************************************************************//*!
	\brief    Reassigns the root path using the executable path
	 
	\param    .exe path
	
	*//**********************************************************************************/
	static void ReassignRootPath(const std::string& exePath);

	static void SelectProjectRootPath();
private:
	static void ProjectViewPopUpOptions();
	static void RenamePopUp();
	static void DeletePopUp();
	static void CopyItem();
	static void PasteItem();
private:

};

