/*****************************************************************//**
 * \file   Editor.h
 * \brief  contains all the editor widgets and controls which to 
 *		   display
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
 * 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "testclass.h"
#include <rapidjson/ostreamwrapper.h>//for ostreamwrapper
#include <rapidjson/prettywriter.h>//for prettywriter
#include <functional>//std::function
#include <stack>//std::stack
#include <imgui.h>
//widgets
#include "HierarchyView.h"
#include "InspectorView.h"
#include "ProjectFolderView.h"
#include "ProjectRootView.h"
#include "WarningView.h"
#include "ToolbarView.h"

#include "Widgets/LoggingView.h"
#include "Widgets/KeyBindingView.h"
#include "Widgets/StyleEditorView.h"

#include "ActionStack/EditorActionStack.h"
#include "Engine/Memory/BufferAllocator.h"

enum class GUIACTIVE_FLAGS:int
{
	INSPECTOR_ACTIVE = 1,
	PROJECTHIRECHY_ACTIVE = 2,
	PROJECTFOLDERVIEW_ACTIVE = 4,
	PROJECTVIEW_ACTIVE = 8,
};
enum class KEY_ACTIONS
{
	RENAME_ITEM = 1,
	DELETE_ITEM,
	HIDE_INSPECTOR,
	HIDE_PROJECTHIRECHY,
	HIDE_PROJECTFOLDERVIEW,
	HIDE_PROJECTVIEW,
};
class Editor
{
public:
	Editor(const std::string& root = "./");
	~Editor();
	//uses the (or) operator to set the flag
	void SetGUIActive(GUIACTIVE_FLAGS flag) { m_activeFlagGUI |= static_cast<int>(flag); }
	//uses the (xor) operator to set the flag
	void SetGUIInactive(GUIACTIVE_FLAGS flag) { m_activeFlagGUI ^= static_cast<int>(flag); }

	void MenuBarUI();

	void HotKeysUpdate();
	void ShowAllWidgets();



private:

	//the 2 functions will be moved out of this class
	void SaveData();
	void LoadData(const char* dir);

public:

	static testclass s_rootnode;
	static std::vector<testclass> s_testList;
	static std::map<KEY_ACTIONS, unsigned int> s_hotkeymapping;

	//for copy and pasting 
	static std::pair<std::string, void* > s_copyPayload;
	static engine::BufferAllocator s_payloadBufferAllocator;

	
private:

	int m_activeFlagGUI = 0;

	HierarchyView m_heirarchy_view;
	InspectorView m_inspector_view;
	ProjectFolderView m_projectfolder_view;
	ProjectRootView m_projectroot_view;
	LoggingView m_logging_view;
	WarningView m_warning_view;
	ActionStack m_action_stack;
	KeyBindingView m_keyboard_view;
	ToolbarView m_toolbar_view;
	StyleEditorView m_styleEditor_view;
	//remb to change the presets TODO: change the presets
	bool logging_widget = true;
	bool action_widget = false;
	bool keyboardview_widget = false;
	bool styleeditor_widget = false;
};

