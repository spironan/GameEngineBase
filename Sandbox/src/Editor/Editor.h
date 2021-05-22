#pragma once
#include "pch.h"
#include "testclass.h"
#include <rapidjson/ostreamwrapper.h>//for ostreamwrapper
#include <rapidjson/prettywriter.h>//for prettywriter
enum class GUIACTIVE_FLAGS
{
	INSPECTOR_ACTIVE = 1,
	PROPERTYEDITOR_ACTIVE = 2,
	PROJECTHIRECHY_ACTIVE = 4,
	PROJECTVIEW_ACTIVE = 8
};
enum class KEY_ACTIONS
{
	RENAME_ITEM = 1,
	DELETE_ITEM,
};
class Editor
{
public:
	Editor();
	~Editor();
	//uses the (or) operator to set the flag
	void SetGUIActive(GUIACTIVE_FLAGS flag) { m_activeFlagGUI |= static_cast<int>(flag); }
	//uses the (xor) operator to set the flag
	void SetGUIInactive(GUIACTIVE_FLAGS flag) { m_activeFlagGUI ^= static_cast<int>(flag); }

	void UpdateUI();
	void TestFunction();
private:
	void ShowObject(testclass& tc);
	void PathDir(std::filesystem::path& entry, std::string& path);
	void ProjectFile(const std::string& path, std::string& selected_dir);
	void PreviewFolder(std::string& path);

	void SaveHirechy(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	void SaveObject(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	void SaveData();
	
	void LoadData(const char* dir);
	void ReadData();

	void HierarchyPopUp();
	void FileWindowPopUp();
private:
	int m_activeFlagGUI = 0;

	testclass* m_focused = nullptr;
	bool m_editing = false;//hirecrchy


	std::string m_currentPath;
	std::vector<testclass> testList;
	std::map<KEY_ACTIONS, unsigned int> m_hotkeymapping;
};