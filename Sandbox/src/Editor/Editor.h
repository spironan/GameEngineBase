#pragma once
#include "testclass.h"
#include <rapidjson/ostreamwrapper.h>//for ostreamwrapper
#include <rapidjson/prettywriter.h>//for prettywriter

#include "HeirarchyView.h"
#include "InspectorView.h"
#include "ProjectFolderView.h"
#include "ProjectRootView.h"
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

	void HotKeysUpdate();
	void UpdateUI();
	void TestFunction();
private:

	void SaveData();
	void LoadData(const char* dir);

public:
	static std::vector<testclass> s_testList;
	static testclass s_rootnode;
	static std::map<KEY_ACTIONS, unsigned int> s_hotkeymapping;



	static testclass* s_FocusedObject;
	

	static std::string s_CurrentPath;
	

private:
	int m_activeFlagGUI = 0;
	testclass* m_focused = nullptr;
	bool m_editing = false;//hirecrchy
	bool m_dragging = false;//hirerchy

	const std::string m_rootPath;
	std::string m_currentPath;
	HeirarchyView m_heirarchy_view;
	InspectorView m_inspector_view;
	ProjectFolderView m_projectfolder_view;
	ProjectRootView m_projectroot_view;


};

